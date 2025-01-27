#pragma once

#include <stdio.h>

struct TSettings;

void ParseIniFile(struct TSettings *, FILE *);
void ParseIniFiles(struct TSettings *settings, const char *files[], char *out);