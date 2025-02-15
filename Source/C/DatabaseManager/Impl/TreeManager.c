#include <Constants.h>
#include <Defines.h>

#include <stdint.h>
#include <stdio.h>

#include <Common/Logger/Logger.h>
#include <sys/stat.h>

#include <DatabaseManager/Impl/TreeManager.h>

#include <Common/Third-party/Json/cJSON.h>

// Стек категории, текущей для вызова хендлера при рекурсивном обходе
static const struct cJSON *g_categoriesStack[MAX_CATEGORIES_DEPTH];
// Метка последней созданной директории в текущем стеке.
static size_t g_maked = 0;

typedef int (*TProductHandler)(size_t, const struct cJSON *);

// Обработчик для передачи в обхо-45д дерева.
static int UpdateHandler(size_t n, struct cJSON *product) {
  DBG(9, "Вызван обработчик для продукта");

  if (!cJSON_IsObject(product))
    RETURN_LOG(-45, "error: UpdateHandler - не ожидаемый тип JSON-объекта, описывающего продукт");

  TODO(
    "Сейчас присходит формирование строкового значения каждый раз заново. Можно сэффективничать");

  uint64_t categories = 0;
  for (size_t i = 0; i < n; ++i) {
    if (!cJSON_IsObject(g_categoriesStack[i]))
      RETURN_LOG(-41, "error: Нода не является объектом (уровень вложенности %d)", i);

    struct cJSON *name = cJSON_GetObjectItem(g_categoriesStack[i], "name");
    if (!name || !cJSON_IsString(name))
      RETURN_LOG(-43, "error: Не получено имя для ноды (уровень вложенности %d)", i);

    struct cJSON *index = cJSON_GetObjectItem(g_categoriesStack[i], "i");
    if (!index || !cJSON_IsNumber(index))
      RETURN_LOG(-44, "error: Не получен индекс для ноды (уровень вложенности %d, имя %s)", i,
                 name->valuestring);

    if (i != 0)
      categories <<= 8;
    categories &= index->valueint;
    char bufName[17];
    sprintf(bufName, "%016lX", categories);
    DBG(10, "Обработка категории %016lX, имя: %s", categories, name->valuestring);
  }

  struct cJSON *name = cJSON_GetObjectItem(product, "name");
  if (!name || !cJSON_IsString(name))
    RETURN_LOG(-46, "error: Не получено имя продукта (категория %016lX)", categories);

  struct cJSON *index = cJSON_GetObjectItem(product, "i");
  if (!index || !cJSON_IsNumber(index))
    RETURN_LOG(-47, "error: Не получен индекс для продукта (категория %016lX, имя %s)", categories,
               name->valuestring, name->valuestring);

  char bufProduct[22];
  sprintf(bufProduct, "%016lX.%04X", categories, index->valueint);
  DBG(9, "Обработка продукта %s, индекс %d", bufProduct, index->valueint);
  mkdir(bufProduct, S_IRWXU);
}

// Для упрощения, решил вынести рекурсивный обход отдельно, что бы не нагружать
// его и не путаться. Для каждого продукта будет вызван хендлер, в глобальном
// скоупе на момент вызова хендлера будет актуальный Список категорий для
// текущего продукта. Индекс указывает на конец стека категорий (за посл. элемент).
static int IterationTroughTree(TProductHandler handler, const struct cJSON *node, size_t i) {
  if (!cJSON_IsObject(node))
    return -41;

  // ориентируемся на наличие вложенных элементов - если они есть, то это
  // категория, и надо проваливаться ещё глубже. если нет - то это уже продукт.
  const cJSON *childs = cJSON_GetObjectItem(node, "childs");
  if (!childs)
    return handler(i, node);
  if (!cJSON_IsArray(childs))
    return -42;

  g_categoriesStack[i++] = node;
  cJSON_ArrayForEach(node, childs) {
    int ret = IterationTroughTree(handler, childs, i);
    if (ret != 0)
      return ret;
  }
  return 0;
}

int UpdateStructure(const struct cJSON *structure) {
}
