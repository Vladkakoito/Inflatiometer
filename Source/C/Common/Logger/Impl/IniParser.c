#include <Common/Logger/Impl/IniParser.h>
#include <Common/Settings.h>
#include <Common/Third-party/Ini/ini.h>

#include <stdlib.h>
#include <string.h>

static int IniHandler(void * user, const char * section, const char* name, const char* value) {
  struct TSettingsLogger * settings = (struct TSettingsLogger*)user;

  if (strcmp(section, "Logger") == 0) {
    if (strcmp(name, "log_level") == 0)
      settings->logLevel = atoi(value);
    else if (strcmp(name, "log_file_name") == 0)
      strcpy(settings->logFileName, value);
    else if (strcmp(name, "err_file_name") == 0)
      strcpy(settings->errFileName, value);
    else if (strcmp(name, "add_file_line"))
      settings->addFileLine = value && strcmp(value, "true") == 0;
    else
      return 0;
  }
  else {
    return 0;
  }
  
  return 1;
}

int ParseIniFileSection(struct TSettingsLogger * settings,  FILE * iniFile) {
  if (ini_parse_file(iniFile, IniHandler, settings) < 0)
    return -1;

  return 0;
}