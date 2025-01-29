# Таргет с обработчиком данных. Получает на вход данные с парсеров. 
# Обрабатывает их и структурирует.
set (THIS_TARGET "CDataHandler")

set (SOURCE_FILES "DataHandler.c")
set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${THIS_TARGET} 
  IniParser
  Logger
  Functions
) 

target_include_directories(${THIS_TARGET} PRIVATE 
  "${CMAKE_SOURCE_DIR}/C"
)

target_compile_definitions(${THIS_TARGET} PRIVATE 
  MY_NAME="${THIS_TARGET}"
)

list(APPEND ALL_TARGETS ${THIS_TARGET})