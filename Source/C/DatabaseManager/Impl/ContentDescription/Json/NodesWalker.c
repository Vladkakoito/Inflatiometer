#include <Constants.h>
#include <Defines.h>

#include <stdint.h>
#include <stdio.h>

#include <Common/Logger/Logger.h>
#include <sys/stat.h>

#include <DatabaseManager/Impl/ContentDescription/NodesWalker.h>

#include <Common/Third-party/Json/cJSON.h>

// Стек категории, текущей для вызова хендлера при рекурсивном обходе
static const struct cJSON *g_categoriesStack[MAX_CATEGORIES_DEPTH];

// Код текущей категории
static uint64_t g_categories = 0;

// Обработчик. регистрируется извне. Вызывается для кажого узла 1 раз.
static TProductHandler g_productHandler;

// Наполнить список тегов из ноды
static int FillArrayTags(uint64_t *resultTags, const cJSON *node) {
  DBG(10, "Наполнение тегов из ноды");
  int i = 0;
  if (!node)
    return 0;
  if (!cJSON_IsArray(node))
    RETURN_LOG(-48, "В ноде в описаниии продуктов имеется элемент \"tags\", однако он не массив");

  cJSON *tag = nullptr;
  cJSON_ArrayForEach(tag, node) {
    if (!tag || !cJSON_IsNumber(tag))
      RETURN_LOG(-49,
                 "Тег ноды в описании продуктов не число (Должен указывать на индекс категории)");
    resultTags[i++] = tag->valueint;
  }

  return i;
}

// Получить данные по продукту и вызвать обработчик
static int ProcessProduct(size_t n, const struct cJSON *product) {
  DBG(9, "Вызван обработчик для продукта");

  if (!cJSON_IsObject(product))
    RETURN_LOG(-45, "error: Не ожидаемый тип JSON-объекта, описывающего продукт");

  struct cJSON *name = cJSON_GetObjectItem(product, "name");
  if (!name || !cJSON_IsString(name))
    RETURN_LOG(-46, "error: Не получено имя продукта (категория %016lX)", g_categories);

  struct cJSON *index = cJSON_GetObjectItem(product, "i");
  if (!index || !cJSON_IsNumber(index))
    RETURN_LOG(-47, "error: Не получен индекс для продукта (категория %016lX, имя %s)",
               g_categories, name->valuestring);

  DBG(9, "Обработка продукта %s, индекс %d", name->valuestring, index->valueint);

  uint64_t tags[MAX_TAGS_COUNT];
  int ret = FillArrayTags(&tags[0], product);
  if (ret >= 0)
    ret = g_productHandler(g_categories, index->valueint, name->valuestring, tags, ret);

  return ret;
}

// Получить данные по категории и вызвать обработчик
static int ProcessCategory(size_t n, const struct cJSON *category) {
  DBG(9, "Вызван обработчик для категории");

  if (!cJSON_IsObject(category))
    RETURN_LOG(-45, "error: Не ожидаемый тип JSON-объекта, описывающего продукт");

  struct cJSON *name = cJSON_GetObjectItem(category, "name");
  if (!name || !cJSON_IsString(name))
    RETURN_LOG(-46, "error: Не получено имя вложенной категории (категория %016lX)", g_categories);

  struct cJSON *index = cJSON_GetObjectItem(category, "i");
  if (!index || !cJSON_IsNumber(index))
    RETURN_LOG(-47, "error: Не получен индекс для категории (категория %016lX, имя %s)",
               g_categories, name->valuestring);

  g_categories &= category->valueint;

  uint64_t tags[MAX_TAGS_COUNT];
  int ret = FillArrayTags(&tags[0], category);
  if (ret >= 0)
    ret = g_productHandler(g_categories, -1 /*product*/, name->valuestring, tags, ret);

  return ret;
}

// Для упрощения, решил вынести рекурсивный обход отдельно, что бы не нагружать
// его и не путаться. Для каждого продукта будет вызван хендлер, в глобальном
// скоупе на момент вызова хендлера будет актуальный Список категорий для
// текущего продукта. Индекс указывает на конец стека категорий (за посл. элемент).
static int IterationTroughTree(const struct cJSON *node, size_t i) {
  if (!cJSON_IsObject(node))
    return -41;

  // ориентируемся на наличие вложенных элементов - если они есть, то это
  // категория, и надо проваливаться ещё глубже. если нет - то это уже продукт.
  const cJSON *childs = cJSON_GetObjectItem(node, "childs");
  if (!childs)
    return ProcessProduct(i, node);
  if (!cJSON_IsArray(childs))
    return -42;

  g_categoriesStack[i++] = node;
  g_categories <<= 8;
  int ret = ProcessCategory(i, node);
  if (ret != 0)
    return ret;

  cJSON_ArrayForEach(node, childs) {
    ret = IterationTroughTree(childs, i);
    if (ret != 0)
      return ret;
  }
  g_categories >>= 8;
  return 0;
}

int ProcessAllNodes(const char *contentStructure) {
  cJSON *dbStructure = cJSON_Parse(contentStructure);
  if (!dbStructure)
    RETURN_LOG(-40, "Не удалось прочитать файл структуры БД: %s", cJSON_GetErrorPtr());

  int ret = IterationTroughTree(dbStructure, 0);
  cJSON_free(dbStructure);
  return ret;
}

void SetProductHandler(TProductHandler h) {
  g_productHandler = h;
}
