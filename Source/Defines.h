#pragma once

#define PRAGMA_MESSAGE(x) _Pragma(#x)
#define TODO(msg)         PRAGMA_MESSAGE(message("TODO: " #msg))
#define RETURN_LOG(code, msg, ...)                                                                 \
  LOG(msg, ##__VA_ARGS__);                                                                         \
  return code

#define EXIT_LOG(code, msg, ...)                                                                   \
  LOG(msg, ##__VA_ARGS__);                                                                         \
  exit(code)

// Парсинг настроек и инициализация логгера -
// почему под одной крышей ? Спросите вы.
// Потому что парсинг настроек может дать ошибку, а выводить её надо в лог.
//  А логгер инициализируется с помощью прочитанных настроек. И так в каждом сервисе.
//  Поэтому, вынес в общий макрос и 2 действия под одну крышу. Отвечу я
#define PARSE_INI_AND_INIT_LOGGER(settings, iniFiles)                                              \
  memset(&settings, 0, sizeof(settings));                                                          \
  char outIniParsing[500] = {'\0'};                                                                \
  ParseIniFiles(&settings, iniFiles, outIniParsing);                                               \
  int ret___ = LOG_INIT(&settings.logger);                                                         \
  if (ret___ < 0)                                                                                  \
    LOG("Ошибка перенаправления лога в файл");                                                     \
  else                                                                                             \
    LOG("Логгер перенаправлен");                                                                   \
  if (outIniParsing[0] != '\0')                                                                    \
    LOG(outIniParsing);                                                                            \
  if (ret___ < 0)                                                                                  \
  return ret___
