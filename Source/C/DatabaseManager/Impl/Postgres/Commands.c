#include <Defines.h>

#include <string.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <DatabaseManager/Impl/Postgres/Commands.h>
#include <DatabaseManager/Impl/Postgres/Connector.h>

#include <libpq-fe.h>

static const char *const kInsertProductQuery =
  "INSERT INTO public.\"Products\" (\"Id\",\"Tags\",\"Name\",\"Categories\")\n"
  "VALUES ($1,$2,$3,$4)\n"
  "ON CONFLICT (\"Id\")\n"
  "DO UPDATE SET "
  "\"Name\"=EXCLUDED.\"Name\","
  "\"Tags\"=EXCLUDED.\"Tags\","
  "\"Categories\"=EXCLUDED.\"Categories\"";

static const char *const kInsertProductStmt = "ip";
static const int kInsertProductParamsCnt = 4;
static const unsigned kInsertProducttTypes[] = {'N', 'A', 'S', 'N'};
static const int kInsertProductsArgsFormat[] = {1, 1, 0, 1};


int MakeCategory(uint64_t categories, const char *name, const uint64_t *tags, int tagsCnt) {
  DBG(9, "Начало создания категории %s в БД Postgres", name);
  PGconn *connection = nullptr;
  int ret = GetConnection(&connection);
  if (ret != 0)
    return ret;

  // Выделение самой добавляемой категории от вложенных
  char categoryId = categories & 0xFF;
  categories >>= 8;

  TODO(Выяснить можно ли так в C.Вроде можно.в C++ то точно нельзя.Тут ещё c big -
       little endian могут быть приколы)
  const char *values[] = {&categoryId, (char *)tags, name, (char *)&categories};

  int tagsLen = 0;
  for (; tags; ++tagsLen)
    ++tags;

  const int lengths[] = {sizeof(categoryId), tagsLen, strlen(name), sizeof(categories)};

  DBG(10, "Параметры подготовлены. Выполнение запроса");
  PGresult *result = PQexecPrepared(connection, kInsertProductStmt, kInsertProductParamsCnt, values,
                                    lengths, kInsertProductsArgsFormat, 1);

  if (PQresultStatus(result) != PGRES_COMMAND_OK)
    RETURN_LOG(-63, "Ошибка вставки %s: %s", name, PQresultErrorMessage(result));

  DBG(10, "Запрос успешно выполнен");
  return 0;
}

int MakeProduct(uint64_t categories, uint32_t product, const char *name, const uint64_t *tags,
                int tagsCnt) {
  DBG(9, "Начало создания продукта %s в БД Postgres", name);
  PGconn *connection = nullptr;
  int ret = GetConnection(&connection);
  if (ret != 0)
    return ret;

  TODO(Выяснить можно ли так в C.Вроде можно.в C++ то точно нельзя.Тут ещё c big -
       little endian могут быть приколы)
  const char *values[] = {(char *)&product, (char *)tags, name, (char *)&categories};

  int tagsLen = 0;
  for (; tags; ++tagsLen)
    ++tags;

  const int lengths[] = {sizeof(product), tagsLen, strlen(name), sizeof(categories)};

  DBG(10, "Параметры подготовлены. Выполнение запроса");
  PGresult *result = PQexecPrepared(connection, kInsertProductStmt, kInsertProductParamsCnt, values,
                                    lengths, kInsertProductsArgsFormat, 1);

  if (PQresultStatus(result) != PGRES_COMMAND_OK)
    RETURN_LOG(-63, "Ошибка вставки %s: %s", name, PQresultErrorMessage(result));

  DBG(10, "Запрос успешно выполнен");
  return 0;
}

int PrepareMakeDB() {
  DBG(7, "Подготовка стейтментов для наполнения БД");
  PGconn *connection = nullptr;
  int ret = GetConnection(&connection);
  if (ret != 0)
    return ret;
  DBG(10, "Вызов функции подготовки стейтемента для Postgres");
  PGresult *result = PQprepare(connection, kInsertProductStmt, kInsertProductQuery,
                               kInsertProductParamsCnt, kInsertProducttTypes);
  if (PQresultStatus(result) != PGRES_COMMAND_OK)
    RETURN_LOG(-62, "Ошибка подготовки стейтмента для вставки продуктов: %s",
               PQresultErrorMessage(result));

  DBG(7, "Стейтменты успешно подготовлены");
  return 0;
}

void FreeMakeDB() {
  PGconn *connection = nullptr;
  if (GetConnection(&connection) == 0)
    PQclosePrepared(connection, kInsertProductStmt);
}
