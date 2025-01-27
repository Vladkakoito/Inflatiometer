#include <Common/Logger/Logger.h>
#include <Common/Settings.h>
#include <Constants.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

static FILE * g_logStream;
static FILE * g_errStream;
static int g_logLevel = 0;
static bool g_addFileLine = false;
static bool g_notAddTs = false;
static bool g_notAddDt = false;

// функция открывает файл на запись. 
// добавляет timestamp в начало и datetime в конец имени файла если надо
static FILE* OpenLogFile(const char* path) {
  const char* filename = strrchr(path, '/');
  if (!filename)
    filename = path;
  else
    ++filename;

  char buffer[TIMESTAMP_LENGTH + MAX_FILEPATH + DATETIME_LENGTH + 2];
  time_t ts = time(nullptr);
  char* cur = strncpy(buffer, path, strlen(path) - strlen(filename));

  if (!g_notAddTs)
    cur += snprintf(cur, TIMESTAMP_LENGTH + 1, "%ld_", ts);

  char* ext = strrchr(filename, '.');
  if (ext)
    *ext++ = '\0';

  cur = strcpy(cur, filename) + strlen(filename);

  if (!g_notAddDt) {
    struct tm * dt = gmtime(&ts);
    cur += strftime(cur, DATETIME_LENGTH + 1, "_%F_%T", dt);
  }

  if (ext)
  {
    *cur++ = '.';
    strcpy(cur, ext);
  }

  return fopen(buffer, "w");
}

int LoggerInit(struct TSettingsLogger * settings) {
  g_logStream = stdout;
  g_errStream = stderr;

  if (!settings)
    return 0;

  if (settings->logFileName[0] != 0)
    g_logStream = OpenLogFile(settings->logFileName);

  if (settings->errFileName[0] != 0)
    g_errStream = OpenLogFile(settings->errFileName);

  if (!g_logStream)
    g_logStream = stdout;
  if (!g_errStream)
    g_errStream = (g_logStream == stdout ? stderr : g_logStream);

  // отключение буферизации для дебага. 
  // перфоманс ни к чему, а своевременные сообщения очень даже
  // например, для дочернего процесса нужно вручную флашить буфер между fork и exec.
  if (g_errStream != g_logStream)
    setvbuf(g_errStream, nullptr /*buffer*/, _IONBF, 0 /*bufSize*/);

  g_logLevel = settings->logLevel;
  g_addFileLine = settings->addFileLine;
  g_notAddTs = settings->notAddTs;
  g_notAddDt = settings->notAddDt;

  return 0;
}

static void Print( const char * msg, FILE * stream, const char * file, int line){
  time_t curTime = time(nullptr);
  struct tm * dt = gmtime(&curTime);
  char timebuf[DATETIME_LENGTH];
  strftime(timebuf, DATETIME_LENGTH, "%F %T", dt);

  if (file)
    fprintf(stream, "[%s] %-*s(%-*d): %s\n", timebuf, MAX_SRC_FILENAME, file, 3, line, msg);
  else
    fprintf(stream, "[%s] %s\n", timebuf, msg);

}

void WriteLog(const char *msg,  ...) {
  if (g_logStream == g_errStream)
    return;
  char buffer[MAX_MESSAGE_LENGTH];
  va_list args;
  va_start(args, msg);
  if (vsnprintf(buffer, MAX_MESSAGE_LENGTH, msg, args) <= 0)
    strcpy(buffer, "LOG ERROR");
  va_end(args);
  Print(buffer, g_logStream, nullptr, 0);
}

void WriteDbg(int level, const char *msg, const char* fileName, long line, ...) {
  if (g_logLevel < level || (level == 0 && g_errStream == stderr))
    return;

  char buffer[MAX_MESSAGE_LENGTH];
  va_list args;
  va_start(args, msg);
  if (vsnprintf(buffer, MAX_MESSAGE_LENGTH, msg, args) <= 0)
    strcpy(buffer, "LOG ERROR");
  va_end(args);

  if (g_addFileLine)
    Print(buffer, g_errStream, fileName, line);
  else 
    Print(buffer, g_errStream, nullptr, 0);
}

void PrintInfo() {
  LOG("Logger:");
  LOG("    Log level: %d", g_logLevel);
  LOG("    Print file and line: %s", g_addFileLine ? "true" : "false");
  LOG("    Do not add timestamp at beginning of filename: %s", g_notAddTs ? "true" : "false");
  LOG("    Do not add timestamp at end of filename: %s", g_notAddDt ? "true" : "false");
}

void LogFlush() {
  fflush(g_logStream);
}

void DbgFlush() {
  fflush(g_errStream);
}