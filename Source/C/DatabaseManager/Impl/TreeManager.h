#pragma once

#include <stdint.h>

typedef int (*TProductHandler)(uint64_t, int32_t, const char *, const char **);

void SetProductHandler(TProductHandler);
int ProcessAllNodes(const char *);
