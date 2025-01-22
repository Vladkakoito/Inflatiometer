#include <Controller/Impl/DoProcess.h>
#include "Controller/Impl/IniParser.h"
#include <Common/Logger/Logger.h>
#include <Common/Settings.h>
#include <Defines.h>

static int PrintConfiguration(const struct TSettings * settings) {
  LOG("\n--------- Конфигурация контроллера ---------");

  LOG_INFO();

  LOG("Системные:");
  for (int i = 0; i < ESYSTEMSETTINGSLAST; ++i)
    LOG("\t%s - %s", SystemMap(i), settings->system.values[i]);

  LOG("Парсеры:");
  for (int i = 0; i < ESTORESLAST; ++i)
  {
    const char * format = i + 1 == ESTORESLAST 
                          ? "\tМагазин: %s - %s\n--------------------------------------------"
                          : "\tМагазин: %s - %s";
    LOG(format, StoresMap(i), settings->parsers.stores[i]);
  }
  
  return 0;
}

static const char * kIniFile = MY_NAME ".ini"; 

int main () {
  FILE * iniFile = fopen(kIniFile, "r");
  if (LoggerInit(iniFile) < 0) {
    fprintf(stderr, "Ошибка инициализации логгера");  
    return 1;
  }
  LOG("Логгер инициализирован");

  if (!iniFile)
    LOG("ВНИМАНИЕ, конфигурационный файл (%s) не прочитан."
        " используются настройки по-умолчанию", kIniFile);

  struct TSettings settings;
  ParseIniFile(&settings, iniFile);
  fclose(iniFile);
  PrintConfiguration(&settings);

  TODO(message("Унести из майна. Сделать запуск по расписанию/команде"));
  TODO("Добавить возможность запуска нескольких процессов одного типа");
  struct TProcessesToRun processes;
  processes.parser = settings.parsers.stores[0];
  processes.dataHandler = settings.dataHandlers.names[0];
  int ret = DoProcess(&processes);
  if (ret < 0)
    return ret;

  return 0;
}