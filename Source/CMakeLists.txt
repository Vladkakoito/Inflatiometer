cmake_minimum_required(VERSION 3.25.0)

include("CMake/Settings.cmake")
include("CMake/CompileFlags.cmake")

project("Inflatiometer" VERSION 1.0)

# Установка  макроса DEBUG для всего проекта
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()

# Подключение поддиректорий из Settings.cmake
set(HAS_C_TARGETS FALSE)
set(HAS_CPP_TARGETS FALSE)
foreach(SOURCES_DIR ${SOURCES_DIRS})
  get_filename_component(ROOT_DIR ${SOURCES_DIR} DIRECTORY)
  if (${ROOT_DIR} STREQUAL "C")
    set(HAS_C_TARGETS TRUE)
  elseif(${ROOT_DIR} STREQUAL "Cpp")
    set(HAS_CPP_TARGETS TRUE)
  endif()
  add_subdirectory("${CMAKE_SOURCE_DIR}/${SOURCES_DIR}")
endforeach()

# Подключение общих директорий только если есть соответствующие таргеты
if (HAS_C_TARGETS)
  add_subdirectory("C/Common")
endif()
if (HAS_CPP_TARGETS)
  add_subdirectory("Cpp/Common")
endif()

add_compile_options(${COMMON_COMPILE_FLAGS})

# Рекурсивная функция. Бежит по всем поддиректориям и собирает таргеты в переменную
function (GetAllTargets RESULT CUR_DIR)
    get_property(TARGETS DIRECTORY ${CUR_DIR} PROPERTY BUILDSYSTEM_TARGETS)
    get_property(SUBDIRS DIRECTORY ${CUR_DIR} PROPERTY SUBDIRECTORIES)

    foreach(SUBDIR ${SUBDIRS})
    GetAllTargets(SUBDIR_TARGETS ${SUBDIR})
      list(APPEND TARGETS ${SUBDIR_TARGETS})
    endforeach()

    set(${RESULT} ${TARGETS} PARENT_SCOPE)
endfunction()

set(ALL_TARGETS "")
GetAllTargets(ALL_TARGETS ${CMAKE_SOURCE_DIR})
message(STATUS "Found targets: ${ALL_TARGETS}") 


# Очистка целевой директории. Туда будут скопированы собранные таргеты и ресурсы.
add_custom_target(CleanOutputDir ALL
  COMMAND ${CMAKE_COMMAND} -E echo "Built targets. Cleanup ${COPY_TO}."
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${COPY_TO}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${COPY_TO}
  DEPENDS ${ALL_TARGETS}
)

# Команда копирования ресурсов, скриптов, и прочей обвязки
foreach(DIR IN LISTS COPY_FROM)
  # Получение имени последнего каталога в копируемом пути. Для создания уникального имени таргета
  string(LENGTH "${DIR}" PATH_LENGTH)
  if(PATH_LENGTH GREATER 0)
    string(SUBSTRING "${DIR}" 1 -1 LAST_CHAR)
    if(LAST_CHAR STREQUAL "/")
      string(SUBSTRING "${DIR}" 0 -2 DIR_WITHOUT_SLASH)
    else()
      set(DIR_WITHOUT_SLASH "${DIR}")
    endif()
    string(FIND "${DIR_WITHOUT_SLASH}" "/" LAST_SLASH_INDEX REVERSE)
    if(NOT LAST_SLASH_INDEX EQUAL -1)
      math(EXPR SUBSTRING_START "${LAST_SLASH_INDEX} + 1")
      string(SUBSTRING "${DIR_WITHOUT_SLASH}" "${SUBSTRING_START}" -1 LAST_NAME)
    else()
      set(LAST_NAME "${DIR_WITHOUT_SLASH}")
    endif()
  else()
    set(LAST_NAME "")
  endif()

  message(STATUS "Additional target: Copy${LAST_NAME}") 
  add_custom_target("Copy${LAST_NAME}" ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_PROJECT_DIR}/${DIR}" "${COPY_TO}"
    DEPENDS CleanOutputDir
  )
endforeach()

# Добавление корневой директории всем. что бы инклудить через <>
foreach(TARGET IN LISTS ALL_TARGETS)
  target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

# Копирование всех таргетов в выходной каталог
foreach(TARGET IN LISTS ALL_TARGETS)
  get_target_property(TARGET_TYPE ${TARGET} TYPE)
  if (NOT ${TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    message(STATUS "Additional target: Copy${TARGET}") 
    add_custom_target("Copy${TARGET}" ALL
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET}> ${COPY_TO}
      DEPENDS CleanOutputDir
    )
  endif()
endforeach()