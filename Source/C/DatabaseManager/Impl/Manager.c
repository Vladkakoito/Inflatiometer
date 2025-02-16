#include <DatabaseManager/Impl/Manager.h>

#ifdef WITH_POSTGRES_DB
#include <DatabaseManager/Impl/Postgres/Commands.h>
#endif


static int MakeNode(uint64_t categories, int32_t product, const char *name, const char **tags) {
  return product == -1 ? PostgresMakeCategory(categories, name, tags)
                       : PostgresMakeProduct(categories, (uint32_t)product, name, tags);
}

int MakeDatabase(struct TSettingsDatabase *settings) {
  return 0;
}
int CheckDatabase(struct TSettingsDatabase *) {
  return 0;
}
