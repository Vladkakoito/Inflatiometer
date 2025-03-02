#pragma once

#include <libpq-fe.h>

struct TSettingsPostgresClient;
int SetSettings(const struct TSettingsPostgresClient *);

int Connect();
void Disconnect();
int GetConnection(PGconn **result);
