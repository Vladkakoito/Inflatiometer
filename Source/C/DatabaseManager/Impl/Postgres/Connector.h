#pragma once

struct TSettingsPostgresClient;
int SetSettings(const struct TSettingsPostgresClient *);

int Connect();
void Disconnect();
