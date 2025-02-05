#include <Common/Logger/Logger.h>
#include "Common/IniParser/IniParser.h"
#include <Common/Settings.h>
#include <Common/Third-party/Json/cJSON.h>
#include <Defines.h>
#include <stdlib.h>

static const char * kIniFile = MY_NAME ".ini";
static const char * kCommonIniFile = "Common.ini";
static const char * kCIniFile = "CCommon.ini";

static cJSON *g_DBStructure = nullptr;

void DBStructureFree() {
  if (g_DBStructure) {
    cJSON_free(g_DBStructure);
    g_DBStructure = nullptr;
  }
}

static void PrintConfiguration(const struct TSettingsDatabaseManager * settings) {
  LOG("\n--------- Конфигурация менеджера БД ---------");
  LOG("\nmanager: %s", settings->selfName);
  LOG("\nstructure: %s", settings->structurePath);
  LOG("\n---------------------------------------------");
}

int main(int argc, char * argv[]) {
  struct TSettings settings;
  const char *iniFiles[] = {kCommonIniFile, kCIniFile, kIniFile, nullptr};
  PARSE_INI_AND_INIT_LOGGER(settings, iniFiles);
  PrintConfiguration(&settings.dbManager);

  if (argc != 2) {
    LOG("Использование: ./%s [command]", argv[0]);
    return 1;
  }

  atexit(DBStructureFree);
  cJSON *dbStructure = cJSON_Parse(settings.dbManager.structurePath);
  if (!dbStructure) {
    LOG("Не удалось прочитать файл структуры БД");
    return -40;
  } 

  LOG("Команда: %s", argv[1]);
  if (strcmp(argv[1], "make") == 0) {
    DBG(8, "Команда определена: %s", argv[1]);
    
  }

}