#ifndef SETTINGS_H
#define SETTINGS_H

#include <Constants.h>

#include <string.h>

#include <Types.h>

enum EStores { EMAGNIT = 0, ESTORESLAST };

enum ESystemSettings { EPYTHONPATH = 0, ESYSTEMSETTINGSLAST };

enum EDatabases { EPOSTGRES = 0, EDATABASELAST };

struct TSettingsParsers {
  char stores[ESTORESLAST][MAX_FILEPATH];
};

struct TSettingsDataHandlers {
  char names[MAX_DATA_HANDLERS][MAX_FILEPATH];
};

struct TSettingsLogger {
  short logLevel;
  bool notAddTs;
  bool notAddDt;
  bool addFileLine;
  bool notStdout;
  char logFileName[MAX_FILENAME];
  char errFileName[MAX_FILENAME];
};

struct TSettingsSystem {
  char values[ESYSTEMSETTINGSLAST][MAX_SYSTEM_PATH];
};

struct TPostgresParams {
  char name[MAX_DB_PARAM_NAME_LENGTH];
  char value[MAX_DB_PARAM_VALUE_LENGTH];
};

struct TSettingsPostgresServer {
  char path[MAX_FILEPATH];
  size_t cnt;
  struct TPostgresParams parameters[MAX_DB_PARAMS_COUNT];
};

struct TSettingsPostgresClient {
  size_t cnt;
  struct TPostgresParams parameters[MAX_DB_PARAMS_COUNT];
};

struct TSettingsDatabase {
  char manager[MAX_FILEPATH];
  char contentDescriptionPath[MAX_FILEPATH];
  char path[MAX_FILEPATH];
  enum EDatabases type;
  struct TSettingsPostgresServer server;
  struct TSettingsPostgresClient client;
};

struct TSettings {
  struct TSettingsLogger logger;
  struct TSettingsParsers parsers;
  struct TSettingsSystem system;
  struct TSettingsDataHandlers dataHandlers;
  struct TSettingsDatabase db;
};

// Маппинг наимнований настроек из системы
static inline const char *SystemMap(enum ESystemSettings storeType) {
  switch (storeType) {
    case EPYTHONPATH:
      return "python";
    default:
      return nullptr;
  }
}

// Маппинг наименований магазинов
static inline const char *StoresMap(enum EStores storeType) {
  switch (storeType) {
    case EMAGNIT:
      return "magnit";
    default:
      return nullptr;
  }
}

// Найти настройку по имени
static inline enum ESystemSettings FindSetting(const char *settingName) {
  for (int i = 0; i < ESYSTEMSETTINGSLAST; ++i) {
    if (strcmp(settingName, SystemMap(i)) == 0)
      return i;
  }
  return ESYSTEMSETTINGSLAST;
}

// Найти магазин по имени
static inline enum EStores FindStore(const char *storeName) {
  for (int i = 0; i < ESTORESLAST; ++i) {
    if (strcmp(storeName, StoresMap(i)) == 0)
      return i;
  }
  return ESTORESLAST;
}

// Маппинг наименований БД
static inline const char *DbMap(enum EDatabases dbType) {
  switch (dbType) {
    case EPOSTGRES:
      return "postgres";
    default:
      return nullptr;
  }
}

// Найти БД по имени
static inline enum EDatabases FindDb(const char *dbName) {
  for (int i = 0; i < EDATABASELAST; ++i) {
    if (strcmp(dbName, DbMap(i)) == 0)
      return i;
  }
  return EDATABASELAST;
}

#endif
