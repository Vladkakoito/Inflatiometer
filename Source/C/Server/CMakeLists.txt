# Таргет с сервером. Слушает команды, обрабатывает их. 
set (THIS_TARGET "CServer")

set (SOURCE_FILES "Server.c")
set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})

list(APPEND ALL_TARGETS ${THIS_TARGET})