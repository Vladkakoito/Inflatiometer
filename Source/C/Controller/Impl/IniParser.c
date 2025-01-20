#include <Common/Logger/Logger.h>
#include <Common/Settings.h>
#include <Common/Third-party/Ini/ini.h>
#include <Controller/Impl/IniParser.h>

#include <stdint.h>
#include <string.h>

static const char * kSectionSystem = "System";
static const char * kSectionParsers = "Parsers";

static void SectionSystemHandler(struct TSettingsSystem * settings, const char* name, const char* value) {
  enum ESystemSettings setting = FindSetting(name);
  if (setting == ESYSTEMSETTINGSLAST)
    LOG("Неизвестное значение конфига: [%s]:%s", kSectionSystem, name);
  else
    strcpy(settings->values[setting], value);
}

static void SectionParsersHandler(struct TSettingsParsers * settings, const char* name, const char* value) {
  enum EStores store = FindStore(name);
  if (store == ESTORESLAST)
    LOG("Неизвестное значение конфига: [%s]:%s", kSectionParsers, name);
  else
    strcpy(settings->stores[store], value);
}

static int CommonHandler(void * user, const char * section, const char* name, const char* value) {
  struct TSettings * settings = (struct TSettings *)user;

  if (strcmp(section, kSectionSystem) == 0)
    SectionSystemHandler(&settings->system, name, value);
  else if (strcmp(section, kSectionParsers) == 0)
    SectionParsersHandler(&settings->parsers, name, value);

  return 0;
}

void ParseIniFile(struct TSettings * settings, FILE * iniFile) {
  ini_parse_file(iniFile, CommonHandler, settings);
}