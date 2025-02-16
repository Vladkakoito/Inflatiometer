#pragma once

struct TSettingsDatabase;

int RunDBManager(const struct TSettingsDatabase *);
void StopDbManager();

int DBCheck();
