#include <Controller/Impl/DoProcess.h>
#include "Common/IniParser/IniParser.h"
#include <Common/Logger/Logger.h>
#include <Common/Settings.h>
#include <Defines.h>
#include <stdio.h>
#include <unistd.h>

static const char * kIniFile = MY_NAME ".ini"; 
TODO(Вынести эти имена в общее место)
static const char * kCommonIniFile = "Common.ini"; 
static const char * kCIniFile = "CCommon.ini"; 

static int PrintConfiguration(const struct TSettings * settings) {
  LOG("\n--------- Конфигурация контроллера ---------");

  LOG_INFO();

  LOG("Системные:");
  for (int i = 0; i < ESYSTEMSETTINGSLAST; ++i)
    LOG("    %s - %s", SystemMap(i), settings->system.values[i]);

  LOG("Парсеры:");
  for (int i = 0; i < ESTORESLAST; ++i)
  {
    const char * format = i + 1 == ESTORESLAST 
                          ? "    Магазин: %s - %s\n--------------------------------------------\n"
                          : "    Магазин: %s - %s";
    LOG(format, StoresMap(i), settings->parsers.stores[i]);
  }
  
  return 0;
}



int main () {
  // инициализация в консоль сначала. что бы уже можно было пользоваться. потом перенаправится из настроек.
  if (LOG_INIT(nullptr) < 0) {
    fprintf(stderr, "Ошибка инициализации логгера");  
    return 1;
  }
  LOG("Старт контроллера... Дальнейший лог будет выводится согласно настройкам.");

  // парсинг всех файлов по очереди. приоритетные настройки в конце
  struct TSettings settings;
  memset(&settings, 0, sizeof(settings));
  const char *iniFiles[] = {kCommonIniFile, kCIniFile, kIniFile, nullptr};
  char outIniParsing[500] = {'\0'};
  ParseIniFiles(&settings, iniFiles, outIniParsing);

  if (LOG_INIT(&settings.logger) < 0)
    LOG("Ошибка перенаправления лога в файл");
  else
    LOG("Логгер перенаправлен");
  if (outIniParsing[0] != '\0')
    LOG(outIniParsing);

  PrintConfiguration(&settings);
  DBG(1, "Главный процесс: %d", (long)getpid());

  TODO(Унести из майна. Сделать запуск по расписанию/команде)
  TODO(Добавить возможность запуска нескольких процессов одного типа)
  struct TProcessesToRun processes;
  processes.parser = settings.parsers.stores[0];
  processes.dataHandler = settings.dataHandlers.names[0];
  int ret = DoProcess(&processes);
  if (ret < 0)
    return ret;

  return 0;
}