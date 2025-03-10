#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Common/IniParser/IniParser.h>
#include <Common/Settings.h>

#include <Common/Third-party/Ini/ini.h>

static const char *kSectionSystem = "System";
static const char *kSectionParsers = "Parsers";
static const char *kSectionLogger = "Logger";
static const char *kSectionDatabase = "Database";
static const char *kSectionPostgresServer = "PostgresServer";
static const char *kSectionPostgresClient = "PostgresClient";

static int SectionLoggerHandler(struct TSettingsLogger *settings, const char *name,
                                const char *value) {
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
  else if (strcmp(name, "not_stdout") == 0)
    settings->notStdout = true;

  return 0;
}

static int SectionPostgresServer(struct TSettingsPostgresServer *settings, const char *name,
                                 const char *value) {
  if (strcmp(name, "path") == 0)
    strcpy(settings->path, value);
  else if (strcpy(settings->parameters[settings->cnt].name, name) &&
           strcpy(settings->parameters[settings->cnt++].value, value))
    ++settings->cnt;
  return 0;
}

static int SectionPostgresClient(struct TSettingsPostgresClient *settings, const char *name,
                                 const char *value) {
  if (strcpy(settings->parameters[settings->cnt].name, name) &&
      strcpy(settings->parameters[settings->cnt++].value, value))
    ++settings->cnt;

  return 0;
}

static int SectionDB(struct TSettingsDatabase *settings, const char *name, const char *value) {
  if (strcmp(name, "manager") == 0)
    strcpy(settings->manager, value);
  else if (strcmp(name, "content_description") == 0)
    strcpy(settings->contentDescriptionPath, value);
  else if (strcmp(name, "path") == 0)
    strcpy(settings->path, value);
  else if (strcmp(name, "type") == 0)
    settings->type = FindDb(value);
  else if (strcmp(name, "unix_socket_directories") == 0)
    strcpy(settings->path, value);
  ;

  return 0;
}

static void SectionSystemHandler(struct TSettingsSystem *settings, const char *name,
                                 const char *value) {
  enum ESystemSettings setting = FindSetting(name);
  if (setting != ESYSTEMSETTINGSLAST)
    strcpy(settings->values[setting], value);
}

static void SectionParsersHandler(struct TSettingsParsers *settings, const char *name,
                                  const char *value) {
  enum EStores store = FindStore(name);
  if (store != ESTORESLAST)
    strcpy(settings->stores[store], value);
}

static int CommonHandler(void *user, const char *section, const char *name, const char *value) {
  struct TSettings *settings = (struct TSettings *)user;

  if (strcmp(section, kSectionSystem) == 0)
    SectionSystemHandler(&settings->system, name, value);
  else if (strcmp(section, kSectionParsers) == 0)
    SectionParsersHandler(&settings->parsers, name, value);
  else if (strcmp(section, kSectionLogger) == 0)
    SectionLoggerHandler(&settings->logger, name, value);
  else if (strcmp(section, kSectionDatabase) == 0)
    SectionDB(&settings->db, name, value);
  else if (strcmp(section, kSectionPostgresServer) == 0)
    SectionPostgresServer(&settings->db.server, name, value);
  else if (strcmp(section, kSectionPostgresClient) == 0)
    SectionPostgresClient(&settings->db.client, name, value);

  return 0;
}

void ParseIniFile(struct TSettings *settings, FILE *iniFile) {
  ini_parse_file(iniFile, CommonHandler, settings);
}

static bool ParseOneIniFile(struct TSettings *settings, const char *file) {
  FILE *iniFile = fopen(file, "r");
  if (!iniFile)
    return false;

  ParseIniFile(settings, iniFile);
  fclose(iniFile);
  return true;
}

void ParseIniFiles(struct TSettings *settings, const char *files[], char *out) {
  while (*files) {
    if (!ParseOneIniFile(settings, *files))
      sprintf(out, "ВНИМАНИЕ, конфигурационный файл (%s) не прочитан.\n", *files);
    ++files;
  }
}
