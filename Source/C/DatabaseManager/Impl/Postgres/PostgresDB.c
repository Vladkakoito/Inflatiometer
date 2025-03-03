#include <DatabaseManager/Impl/Postgres/Commands.h>
#include <DatabaseManager/Impl/Postgres/Connector.h>
#include <DatabaseManager/Impl/Postgres/PostgresDB.h>

int PostgresMakeCategory(uint64_t categories, const char *name, const uint64_t *tags, int tagsCnt) {
  return MakeCategory(categories, name, tags, tagsCnt);
}

int PostgresMakeProduct(uint64_t categories, uint32_t product, const char *name,
                        const uint64_t *tags, int tagsCnt) {
  return MakeProduct(categories, product, name, tags, tagsCnt);
}

int PostgresPrepareMakeDB() {
  return PrepareMakeDB();
}

void PostgresFreeMakeDB() {
  FreeMakeDB();
}
