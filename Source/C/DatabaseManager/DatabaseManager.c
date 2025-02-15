#include <Defines.h>

#include <stdlib.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include "Common/IniParser/IniParser.h"
#include <Common/Third-party/Json/cJSON.h>

static const char *kIniFile = MY_NAME ".ini";
static const char *kCommonIniFile = "Common.ini";
static const char *kCIniFile = "CCommon.ini";

static cJSON *g_DBStructure = nullptr;

void DBStructureFree() {
  if (g_DBStructure) {
    cJSON_free(g_DBStructure);
    g_DBStructure = nullptr;
  }
}

static void PrintConfiguration(const struct TSettingsDatabase *settings) {
  LOG("\n--------- Конфигурация менеджера БД ---------");
  LOG("\nmanager: %s", settings->path);
  LOG("\ncontent description: %s", settings->contentDescriptionPath);
  LOG("\ncmanager: %s", settings->manager);
  LOG("\nserver: %s", settings->server);
  LOG("\ntype: %s", DbMap(settings->type));
  LOG("\n---------------------------------------------");
}

int main(int argc, char *argv[]) {
  struct TSettings settings;
  const char *iniFiles[] = {kCommonIniFile, kCIniFile, kIniFile, nullptr};
  PARSE_INI_AND_INIT_LOGGER(settings, iniFiles);
  PrintConfiguration(&settings.db);

  if (argc != 2) {
    LOG("Использование: ./%s [command]", argv[0]);
    return 1;
  }

  atexit(DBStructureFree);
  cJSON *dbStructure = cJSON_Parse(settings.db.contentDescriptionPath);
  if (!dbStructure) {
    LOG("Не удалось прочитать файл структуры БД: %s", cJSON_GetErrorPtr());
    return -40;
  }

  LOG("Команда: %s", argv[1]);
  if (strcmp(argv[1], "make") == 0) {
    DBG(8, "Команда определена: %s", argv[1]);
  }
}
