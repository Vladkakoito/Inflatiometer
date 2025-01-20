# Таргет с сервером. Слушает команды, обрабатывает их. 
set (THIS_TARGET "CppServer")

set (SOURCE_FILES "Server.cpp")
set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})