#pragma once

struct TSettingsLogger;

int LoggerInit(struct TSettingsLogger *settings);
void WriteLog(const char*, ...);
void WriteDbg(int, const char *, const char*, long, ...);
void PrintInfo();
void LogFlush();
void DbgFlush();

#define LOG(msg, ...) WriteLog(msg, ##__VA_ARGS__), \
                      DBG(0, msg, ##__VA_ARGS__)

#ifndef DEBUG
#define DBG(level, msg, ...) WriteDbg(level, msg, __FILE_NAME__, __LINE__,  ##__VA_ARGS__)
#else
#define DBG(level, msg, ...) (void)0
#endif

#define LOG_INFO() PrintInfo()
#define LOG_INIT(settings) LoggerInit(settings)

#define LOG_FLUSH() LogFlush(), DbgFlush()