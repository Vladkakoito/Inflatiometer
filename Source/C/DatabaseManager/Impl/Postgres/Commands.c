#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <DatabaseManager/Impl/Postgres/Commands.h>
#include <DatabaseManager/Impl/Postgres/Connector.h>

#include <libpq-fe.h>

static const char *const kInsertStmt =
  "INSERT INTO public.\"Products\" (\"Id\",\"Tags\",\"Name\")\n"
  "VALUES ($1,$2,$3)\n"
  "ON CONFLICT (\"Id\")\n"
  "DO UPDATE SET \"Name\"=EXCLUDED.\"Name\",\"Tags\" = EXCLUDED.\"Tags\";";

int MakeCategory(uint64_t categories, const char *name, const char **tags) {
  DBG(9, "Начало создания категории %s в БД Postgres", name);
  PGconn *connection = nullptr;
  int ret = GetConnection(&connection);
  if (ret != 0)
    return ret;

  // надо подумать где подготовить стейтмент. А потом освободить. Эта функция - колбек при обходе json-а структуры БД
}

int MakeProduct(uint64_t categories, uint32_t product, const char *name, const char **tags) {
  return 0;
}
