#include "Constants.h"
#include <Common/Logger/Impl/IniParser.h>
#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

FILE * g_logStream;
FILE * g_errStream;
int g_logLevel = 0;
bool g_addFileLine = false;

int LoggerInit(FILE * iniFile) {
  g_logStream = stdout;
  g_errStream = stderr;

  struct TSettingsLogger settings = {.logLevel=0, .addFileLine=false};
  memset(settings.logFileName, '\0', MAX_FILENAME);
  memset(settings.errFileName, '\0', MAX_FILENAME);

  if (!iniFile || ParseIniFileSection(&settings, iniFile) < 0)
    return 0;

  if (settings.logFileName[0] != '\0')
    g_logStream = fopen(settings.logFileName, "w");

  if (settings.errFileName[0] != '\0')
    g_errStream = fopen(settings.errFileName, "w");
  else
    g_errStream = g_logStream;
  
  g_logLevel = settings.logLevel;
  g_addFileLine = settings.addFileLine;

  return 0;
}

static void Print( const char * msg, FILE * stream, const char * file, int line){
  time_t curTime = time(nullptr);
  struct tm * dt = gmtime(&curTime);
  char timebuf[20];
  strftime(timebuf, 20, "%F %T", dt);

  if (file)
    fprintf(stream, "[%s] %-*s(%d): %s\n", timebuf, 30 - (int)strlen(file), file, line, msg);
  else
    fprintf(stream, "[%s] %s\n", timebuf, msg);

}

void WriteLog(const char *msg, ...) {
  char buffer[MAX_MESSAGE_LENGTH];
  va_list args;
  va_start(args, msg);
  if (vsnprintf(buffer, MAX_MESSAGE_LENGTH, msg, args) <= 0)
    strcpy(buffer, "LOG ERROR");
  va_end(args);
  Print(buffer, g_logStream, nullptr, 0);
}

void WriteDbg(int level, const char *msg, ...) {
  if (g_logLevel < level)
    return;

  char buffer[MAX_MESSAGE_LENGTH];
  va_list args;
  va_start(args, msg);
  if (vsnprintf(buffer, MAX_MESSAGE_LENGTH, msg, args) <= 0)
    strcpy(buffer, "LOG ERROR");
  va_end(args);

  if (g_addFileLine)
    Print(buffer, g_errStream, __FILE_NAME__, __LINE__);
  else 
    Print(buffer, g_errStream, nullptr, 0);
}

void PrintInfo() {
  LOG("Logger:");
  LOG("\tLog level: %d", g_logLevel);
  LOG("\tPrint file and line: %s", g_addFileLine ? "true" : "false");
}