#pragma once

#include <stdint.h>

int PostgresMakeCategory(uint64_t categories, const char *name, const uint64_t *tags, int tagsCnt);

int PostgresMakeProduct(uint64_t categories, uint32_t product, const char *name,
                        const uint64_t *tags, int tagsCnt);

int PostgresPrepareMakeDB();

void PostgresFreeMakeDB();
