#include <Common/IniParser/IniParser.h>
#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <string.h>
#include <unistd.h>

static const char * kIniFile = MY_NAME ".ini"; 
static const char * kCommonIniFile = "Common.ini"; 
static const char * kCIniFile = "CCommon.ini";

static int PrintConfiguration(const struct TSettings * settings) {
  LOG("\n--------- Конфигурация обработчика данных ---------");
  LOG("---------------------------------------------------\n");
  return 0;
}

int main() {
  // парсинг всех файлов по очереди. приоритетные настройки в конце
  struct TSettings settings;
  memset(&settings, 0, sizeof(settings));
  const char *iniFiles[] = {kCommonIniFile, kCIniFile, kIniFile, nullptr};
  char outIniParsing[500] = {'\0'};
  ParseIniFiles(&settings, iniFiles, outIniParsing);

  if (LOG_INIT(&settings.logger) < 0)
    LOG("Ошибка инициализации лога в файл");
  else
    LOG("Логгер инициализирован");
  if (outIniParsing[0] != '\0')
    LOG(outIniParsing);

  PrintConfiguration(&settings);

  DBG(1, "PID: %d", (long)getpid());

  
  
  return 0;
}