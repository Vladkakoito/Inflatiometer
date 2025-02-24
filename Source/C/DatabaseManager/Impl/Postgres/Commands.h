#pragma once

#include <stdint.h>

int MakeCategory(uint64_t categories, const char *name, const char **tags);

int MakeProduct(uint64_t categories, uint32_t product, const char *name, const char **tags);
