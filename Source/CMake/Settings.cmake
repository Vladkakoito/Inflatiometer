# Файл с настройками сборки проекта

# Каталог проекта
set(CMAKE_PROJECT_DIR "${CMAKE_SOURCE_DIR}/..")
message(STATUS "CMAKE_PROJECT_DIR: ${CMAKE_PROJECT_DIR}")

# Каталог с бинарниками
set(CMAKE_BINARY_DIR "${CMAKE_PROJECT_DIR}/Bin/${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

# Каталоги, откуда и куда будет всё копироваться после сборки всех таргетов
set(COPY_FROM "Source/Python" "Resources" "Configs")
set(COPY_TO "${CMAKE_BINARY_DIR}")

# Каталоги с исходниками
set(CONTROLLER "C/Controller" "Cpp/Controller")
set(SERVER "C/Server" "Cpp/Server")
set(DATA_HANDLER "C/DataHandler" "Cpp/DataHandler")
set(SOURCES_DIRS ${CONTROLLER} ${SERVER} ${DATA_HANDLER})

# Компиляторы
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "clang++")

# Стандарты
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Флаги компиляции
set(C_FLAGS 
  "-Wall"
  "-Wextra"
  "-Wunknown-pragmas"
)