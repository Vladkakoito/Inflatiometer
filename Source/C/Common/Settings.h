#ifndef SETTINGS_H
#define SETTINGS_H

#include <Types.h>
#include <Constants.h>
#include <string.h>

enum EStores {
  EMAGNIT = 0,
  ESTORESLAST
};

enum ESystemSettings {
  EPYTHONPATH = 0,
  ESYSTEMSETTINGSLAST
};

struct TSettingsParsers {
  char stores[ESTORESLAST][MAX_FILEPATH];
};

struct TSettingsLogger {
  short logLevel;
  bool addFileLine;
  char logFileName[MAX_FILENAME];
  char errFileName[MAX_FILENAME];
};

struct TSettingsSystem {
  char values[ESYSTEMSETTINGSLAST][MAX_SYSTEM_PATH];
};

struct TSettings {
  struct TSettingsLogger logger;
  struct TSettingsParsers parsers;
  struct TSettingsSystem system;
};

// Маппинг наимнований настроек из системы
static inline const char * SystemMap(enum ESystemSettings storeType) {
  switch (storeType) {
    case EPYTHONPATH:
      return "python";
    default:
      return nullptr;  
  }
}

// Маппинг наименований магазинов
static inline const char * StoresMap(enum EStores storeType) {
  switch (storeType) {
    case EMAGNIT:
      return "magnit";
    default:
      return nullptr;
  }
}

// Найти настройку по имени
static inline enum ESystemSettings FindSetting(const char * settingName) {
  for (int i = 0; i < ESYSTEMSETTINGSLAST; ++i) {
    if (strcmp(settingName, SystemMap(i)))
      return i;
  }
  return ESYSTEMSETTINGSLAST;
}

// Найти магазин по имени
static inline enum EStores FindStore(const char * storeName) {
  for (int i = 0; i < ESTORESLAST; ++i) {
    if (strcmp(storeName, StoresMap(i)))
      return i;
  }
  return ESTORESLAST;
}

#endif