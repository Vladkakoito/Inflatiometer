#pragma once

#include <stdint.h>

// Обработчик для каждой ноды вызван будет.
/*
  Аргументы 
    - набор категорий, куда вложена нода
    - индекс продукта. -1 - эта нода категория
    - имя ноды
    - теги ноды. заканчиваются nullptr-ом
  Освобождать ничего не надо, для сохранения - копируем
*/
typedef int (*TProductHandler)(uint64_t, int32_t, const char *, const uint64_t *, int tagsCnt);

void SetProductHandler(TProductHandler);
int ProcessAllNodes(const char *);
