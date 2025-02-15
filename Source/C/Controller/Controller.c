#include <Defines.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <Controller/Impl/DoProcess.h>

#include "Common/IniParser/IniParser.h"

static const char *kIniFile = MY_NAME ".ini";
TODO(Вынести эти имена в общее место)
static const char *kCommonIniFile = "Common.ini";
static const char *kCIniFile = "CCommon.ini";

// Флаг, была ли команда остановки от пользователя.
static bool g_isStop = false;

static void PrintConfiguration(const struct TSettings *settings) {
  LOG("\n--------- Конфигурация контроллера ---------");

  LOG_INFO();

  LOG("Системные:");
  for (int i = 0; i < ESYSTEMSETTINGSLAST; ++i)
    LOG("    %s - %s", SystemMap(i), settings->system.values[i]);

  LOG("Парсеры:");
  for (int i = 0; i < ESTORESLAST; ++i) {
    const char *format = i + 1 == ESTORESLAST
                         ? "    Магазин: %s - %s\n--------------------------------------------\n"
                         : "    Магазин: %s - %s";
    LOG(format, StoresMap(i), settings->parsers.stores[i]);
  }
}


int RunDBServer(const struct TSettingsDatabase *settings) {
  DBG(2, "Запускается сервер БД");

  LOG_FLUSH(); // что бы буфер не попал и в родителя и в потомка

  int pid = fork();
  if (pid < 0)
    RETURN_LOG(-14, "Ошибка вызова fork в RunDBServer: %s", strerror(errno));

  // родительский процесс
  if (pid > 0)
    return 0;

  switch (settings->type) {
    case EPOSTGRES:
      if (settings->path[0] == '\0') // настройки должны быть инициализированы нулями
        EXIT_LOG(-16, "Ошибка запуска сервера БД: не указан путь до базы данных");

      if (execlp(settings->server, settings->server, "-D", settings->path, nullptr) >= 0)
        exit(0);

      exit(-16);
      break;

    default:
      EXIT_LOG(-15, "Указанный сервер БД(%s) не поддерживается");
  }
  exit(-1);
}


int main() {
  // инициализация в консоль сначала. что бы уже можно было пользоваться. потом перенаправится из настроек.
  if (LOG_INIT(nullptr) < 0) {
    fprintf(stderr, "Ошибка инициализации логгера");
    return 1;
  }
  LOG("Старт контроллера... Дальнейший лог будет выводится согласно настройкам.");

  // парсинг всех файлов по очереди. приоритетные настройки в конце
  struct TSettings settings;
  const char *iniFiles[] = {kCommonIniFile, kCIniFile, kIniFile, nullptr};
  PARSE_INI_AND_INIT_LOGGER(settings, iniFiles);

  PrintConfiguration(&settings);
  DBG(1, "Главный процесс: %d", (long)getpid());

  TODO(Унести из майна.Сделать запуск по расписанию / команде)
  TODO(Добавить возможность запуска нескольких процессов одного типа)
  struct TProcessesToRun processes;
  processes.parser = settings.parsers.stores[0];
  processes.dataHandler = settings.dataHandlers.names[0];

  int ret = RunDBServer(&settings.db);

  while (ret == 0 && !g_isStop)
    ret = DoProcess(&processes);

  return ret;
}
