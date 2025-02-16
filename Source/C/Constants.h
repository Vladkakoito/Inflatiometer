#pragma once

// общее
#define MAX_FILENAME       60
#define MAX_FILEPATH       100
#define MAX_PARSERS_COUNT  100
#define MAX_SYSTEM_PATH    100
#define MAX_MESSAGE_LENGTH 200 // сообщение логгера
#define MAX_DATA_HANDLERS  10
#define DATETIME_LENGTH    20
#define TIMESTAMP_LENGTH   11

// структура БД
#define MAX_CATEGORIES_DEPTH 8  // на каждую по байту. хочу, что бы влезло в число.
#define MAX_TAGS_COUNT       10 // максимальное число тегов для продукта (категории)

// сервер БД
#define MAX_DB_PARAM_NAME_LENGTH  20
#define MAX_DB_PARAM_VALUE_LENGTH 50
#define MAX_DB_PARAMS_COUNT       30

// менеджер БД
#define MAX_ANSWER_LENGTH  10
#define MAX_COMMAND_LENGTH 10
