# Таргет с обработчиком данных. Получает на вход данные с парсеров. 
# Обрабатывает их и структурирует.
set (THIS_TARGET "CppDataHandler")

set (SOURCE_FILES "DataHandler.cpp")
set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})