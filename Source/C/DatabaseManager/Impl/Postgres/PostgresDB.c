#include <DatabaseManager/Impl/Postgres/Commands.h>
#include <DatabaseManager/Impl/Postgres/Connector.h>
#include <DatabaseManager/Impl/Postgres/PostgresDB.h>

int PostgresMakeCategory(uint64_t categories, const char *name, const char **tags) {
  return MakeCategory(categories, name, tags);
}

int PostgresMakeProduct(uint64_t categories, uint32_t product, const char *name,
                        const char **tags) {
  return MakeProduct(categories, product, name, tags);
}
