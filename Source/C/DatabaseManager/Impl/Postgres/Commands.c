#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <DatabaseManager/Impl/Postgres/Commands.h>

int MakeCategory(uint64_t categories, const char *name, const char **tags) {
  DBG(9, "Начало создания категории %s в БД Postgres", name);

  return 0;
}

int MakeProduct(uint64_t categories, uint32_t product, const char *name, const char **tags) {
  return 0;
}
