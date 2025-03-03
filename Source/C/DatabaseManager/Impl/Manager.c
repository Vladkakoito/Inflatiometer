#include <Defines.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <DatabaseManager/Impl/ContentDescription/NodesWalker.h>
#include <DatabaseManager/Impl/Manager.h>

#ifdef WITH_POSTGRES_DB
#include <DatabaseManager/Impl/Postgres/PostgresDB.h>
#endif

enum EDatabases g_dbType;

static int MakeNode(uint64_t categories, int32_t product, const char *name, const uint64_t *tags,
                    int tagsCnt) {
  switch (g_dbType) {
    case EPOSTGRES:
      DBG(10, "Запуск обработчика ноды %s", name);
      return product == -1
             ? PostgresMakeCategory(categories, name, tags, tagsCnt)
             : PostgresMakeProduct(categories, (uint32_t)product, name, tags, tagsCnt);
    default:
      RETURN_LOG(-30, "Не поддерживается тип БД из настроек");
  }
  return 0;
}

static int PrepareMakeDB() {
  switch (g_dbType) {
    case EPOSTGRES:
      DBG(8, "Запуск подготовки БД к инициализации структуры");
      return PostgresPrepareMakeDB();
    default:
      RETURN_LOG(-30, "Не поддерживается тип БД из настроек");
  }
  return 0;
}

static int FreeMakeDB() {
  switch (g_dbType) {
    case EPOSTGRES:
      DBG(8, "Освобождение ресурсов после наполнения БД");
      PostgresFreeMakeDB();
      break;
    default:
      RETURN_LOG(-30, "Не поддерживается тип БД из настроек");
  }
  return 0;
}

int CheckDatabase(struct TSettingsDatabase *settings) {
  LOG("Старт проверки БД. Если не найден - будет добавлен. Если уже есть, ничего не будет сделано");
  g_dbType = settings->type;
  SetProductHandler(MakeNode);
  int ret = PrepareMakeDB();
  if (ret != 0)
    return ret;
  ret = ProcessAllNodes(settings->contentDescriptionPath);
  FreeMakeDB();
  return ret;
}
