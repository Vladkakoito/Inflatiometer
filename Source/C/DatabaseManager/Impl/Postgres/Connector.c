#include <Constants.h>
#include <Defines.h>

#include <stdlib.h>
#include <string.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <DatabaseManager/Impl/Postgres/Connector.h>

#include <libpq-fe.h>

static PGconn *g_connection = nullptr;
static char *g_connectionOptions = nullptr;

TODO(Хоть это и только при запуске но может стоит подумать как не копировать параметры лишний раз)
int SetSettings(const struct TSettingsPostgresClient *settings) {
  DBG(2, "Установка параметров БД Postgres");
  DBG(9, "Всего %d параметров", settings->cnt);
  // Параметры в виде "keyword1=value1 keyword2=value2 ..."
  char *params = malloc((settings->cnt * 2 + /*`=` и ` `*/ 2) * MAX_DB_PARAM_NAME_LENGTH);
  char *cur = params;
  for (size_t i = 0; i < settings->cnt; ++i) {
    cur = strcpy(cur, settings->parameters[i].name) + strlen(settings->parameters[i].name);
    *cur = '=';
    cur = strcpy(cur, settings->parameters[i].value) + strlen(settings->parameters[i].value);
    *cur = ' ';
  }
  *cur = '\0';

  DBG(9, "Параметры преобразованы, запуск парсера Postgres для проверки корректности");
  char *err = nullptr;
  PQconninfoParse(params, &err);
  if (err) {
    LOG("Ошибка парсинга параметров %s", err);
    free(err);
    return -60;
  }
  g_connectionOptions = params;

  return g_connectionOptions ? 0 : -60;
}


void FreeParams() {
  DBG(5, "Освобожднение памяти параметров БД Postgres");
  free(g_connectionOptions);
}

int Connect() {
  DBG(3, "Попытка соединения с БД Postgres");
  g_connection = PQconnectdb(g_connectionOptions);
  if (!g_connection)
    RETURN_LOG(-61, "Неудачная попытка соединения с БД");

  return 0;
}


void Disconnect() {
  DBG(3, "Разрыв соединения с БД Postgres");
  FreeParams();
  PQfinish(g_connection);
}
