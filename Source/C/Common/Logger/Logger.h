#pragma once

struct TSettingsLogger;

int LoggerInit(struct TSettingsLogger * settings);
void WriteLog(const char *, ...);
void WriteDbg(int, const char *, ...);
void PrintInfo();

#define LOG(msg, ...) WriteLog(msg, ##__VA_ARGS__), \
                      DBG(0, msg, ##__VA_ARGS__)

#ifdef DEBUG
#define DBG(level, msg, ...) WriteDbg(level, msg, ##__VA_ARGS__)
#else
#define DBG(level, msg, ...) (void)0
#endif

#define LOG_INFO() PrintInfo()
#define LOG_INIT(settings) LoggerInit(settings)