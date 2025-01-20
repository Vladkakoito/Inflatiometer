# Таргет с контроллером. Контроллер - самый главный тут. 
# Запускает и управляет всеми процессами.
set (THIS_TARGET "CppController")

set (SOURCE_FILES "Controller.cpp")
set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})