#include <Defines.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>

#include <DatabaseManager/Impl/ContentDescription/NodesWalker.h>
#include <DatabaseManager/Impl/Manager.h>

#ifdef WITH_POSTGRES_DB
#include <DatabaseManager/Impl/Postgres/PostgresDB.h>
#endif

enum EDatabases g_dbType;

static int MakeNode(uint64_t categories, int32_t product, const char *name, const char **tags) {
  switch (g_dbType) {
    case EPOSTGRES:
      DBG(10, "Запуск обработчика ноды %s", name);
      return product == -1 ? PostgresMakeCategory(categories, name, tags)
                           : PostgresMakeProduct(categories, (uint32_t)product, name, tags);
    default:
      RETURN_LOG(-30, "Не поддерживается тип БД из настроек");
  }
}

int CheckDatabase(struct TSettingsDatabase *settings) {
  LOG("Старт проверки БД. Если не найден - будет добавлен. Если уже есть, ничего не будет сделано");
  g_dbType = settings->type;
  SetProductHandler(MakeNode);
  return ProcessAllNodes(settings->contentDescriptionPath);
}
