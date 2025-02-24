#include <Constants.h>
#include <Defines.h>

#include <stdlib.h>
#include <string.h>

#include <Common/Settings.h>

#include <DatabaseManager/Impl/Postgres/Connector.h>

#include <libpq-fe.h>

static PGconn *g_connection = nullptr;
static PQconninfoOption g_connectionOptions;

TODO(Хоть это и только при запуске, но может стоит подумать как не копировать параметры лишний раз)
void SetSettings(const struct TSettingsPostgresClient *settings) {
  char *keywords = malloc(settings->cnt * MAX_DB_PARAM_NAME_LENGTH);
  char *values = malloc(settings->cnt * MAX_DB_PARAM_VALUE_LENGTH);
  char *curKeywords = keywords;
  char *curValues = values;
  for (size_t i = 0; i < settings->cnt; ++i) {
    curKeywords = strcpy(curKeywords, settings->parameters[i].name);
    curValues = strcpy(curValues, settings->parameters[i].value);
  }
}


void FreeParams() {
}


int CheckConnect() {
}
int Disconnect() {
}
