#include <Defines.h>

#include <stdlib.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include "DatabaseManager/Impl/TreeManager.h"

#include "Common/IniParser/IniParser.h"

static const char *kIniFile = MY_NAME ".ini";
static const char *kCommonIniFile = "Common.ini";
static const char *kCIniFile = "CCommon.ini";

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

  LOG("Команда: %s", argv[1]);
  if (strcmp(argv[1], "make") == 0) {
    DBG(8, "Команда определена: %s", argv[1]);
    return
  }
}
