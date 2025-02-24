#pragma once

struct TSettingsPostgresClient;
void SetSettings(const struct TSettingsPostgresClient *);

int CheckConnect();
int Disconnect();
