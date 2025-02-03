cmake_minimum_required(VERSION 3.25.0)

include("CMake/Settings.cmake")
include("CMake/CompileFlags.cmake")

project("Inflatiometer" VERSION 1.0)

# Установка  макроса DEBUG для всего проекта
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()

# Таргеты сами сюда записываются в своих симейклистах
set(ALL_TARGETS "")

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

message(STATUS "Found targets: ${ALL_TARGETS}") 

# Добавление корневой директории всем. что бы инклудить через <>
foreach(TARGET IN LISTS ALL_TARGETS)
  target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

# Таргет-маркер окончания сборки всех таргетов.
add_custom_target(AllTargets ALL
  COMMAND ${CMAKE_COMMAND} -E echo "Built targets."
  DEPENDS ${ALL_TARGETS}
)

set(COPY_DESTINATION_DIR "@COPY_TO@")
set(COPY_SOURCES_DIRS "@COPY_FROM@")
set(TARGETS_TO_COPY "@ALL_TARGETS@")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/CMake/PostCopy.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/PostCopy.cmake"
    @ONLY
)

add_custom_command(
    TARGET AllTargets
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/PostCopy.cmake"
    COMMENT "Копирование таргетов, ресурсов и прочей обвязки после сборки всех целевых проектов"
)