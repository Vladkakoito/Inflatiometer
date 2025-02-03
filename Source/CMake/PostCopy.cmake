# Рекурсивная функция копирования, копирует только изменённые файлы
function(copy_if_different SOURCE_DIR TARGET_DIR)
    file(GLOB_RECURSE files RELATIVE "${SOURCE_DIR}" "${SOURCE_DIR}/*")
    foreach(file ${files})
        get_filename_component(dir "${file}" DIRECTORY)
        file(MAKE_DIRECTORY "${TARGET_DIR}/${dir}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SOURCE_DIR}/${file}" "${TARGET_DIR}/${file}"
        )
    endforeach()
endfunction()


# Команда копирования ресурсов, скриптов, и прочей обвязки
foreach(DIR IN LISTS COPY_SOURCES_DIRS)
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

  message(STATUS "Copy${LAST_NAME} to ${COPY_DESTINATION_DIR}") 
  copy_if_different("${CMAKE_PROJECT_DIR}/${DIR}" "${COPY_DESTINATION_DIR}")
endforeach() 

# Копирование всех таргетов в выходной каталог
foreach(TARGET IN LISTS TARGETS_TO_COPY)
  get_target_property(TARGET_TYPE ${TARGET} TYPE)
  if (NOT ${TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
    message(STATUS "Additional target: Copy${TARGET}") 
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET}> ${COPY_TO}
    )
  endif()
endforeach()

