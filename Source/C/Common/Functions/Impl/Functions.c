#include <Common/Functions/Functions.h>

#include <string.h>

const char* GetFileName(const char *path) {
  const char *filename = strrchr(path, '/');
  return filename ? filename + 1 : path;
}