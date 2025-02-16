#pragma once

#include <stdint.h>

int PostgresMakeCategory(uint64_t categories, const char *name, const char **tags);

int PostgresMakeProduct(uint64_t categories, uint32_t product, const char *name, const char **tags);
