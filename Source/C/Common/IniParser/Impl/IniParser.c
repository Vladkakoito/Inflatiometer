#include <Common/IniParser/IniParser.h>
#include <Common/Settings.h>
#include <Common/Third-party/Ini/ini.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static const char * kSectionSystem = "System";
static const char * kSectionParsers = "Parsers";
static const char * kSectionLogger = "Logger";

static int SectionLoggerHandler(struct TSettingsLogger * settings, const char* name, const char* value) {

  if (strcmp(name, "log_level") == 0)
    settings->logLevel = atoi(value);
  else if (strcmp(name, "log_file_name") == 0)
    strcpy(settings->logFileName, value);
  else if (strcmp(name, "err_file_name") == 0)
    strcpy(settings->errFileName, value);
  else if (strcmp(name, "add_file_line") == 0)
    settings->addFileLine = value && strcmp(value, "true") == 0;
  else if (strcmp(name, "not_add_ts_before_filenames") == 0)
    settings->notAddTs = value && strcmp(value, "true") == 0;
  else if (strcmp(name, "not_add_dt_after_filenames") == 0)
    settings->notAddDt = value && strcmp(value, "true");
  
  return 0;
}

static void SectionSystemHandler(struct TSettingsSystem * settings, const char* name, const char* value) {
  enum ESystemSettings setting = FindSetting(name);
  if (setting != ESYSTEMSETTINGSLAST)
    strcpy(settings->values[setting], value);
}

static void SectionParsersHandler(struct TSettingsParsers * settings, const char* name, const char* value) {
  enum EStores store = FindStore(name);
  if (store != ESTORESLAST)
    strcpy(settings->stores[store], value);
}

static int CommonHandler(void * user, const char * section, const char* name, const char* value) {
  struct TSettings * settings = (struct TSettings *)user;

  if (strcmp(section, kSectionSystem) == 0)
    SectionSystemHandler(&settings->system, name, value);
  else if (strcmp(section, kSectionParsers) == 0)
    SectionParsersHandler(&settings->parsers, name, value);
  else if (strcmp(section, kSectionLogger) == 0)
    SectionLoggerHandler(&settings->logger, name, value);

  return 0;
}

void ParseIniFile(struct TSettings * settings, FILE * iniFile) {
  ini_parse_file(iniFile, CommonHandler, settings);
}