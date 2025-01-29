# Таргет с сервером. Слушает команды, обрабатывает их. 
set (THIS_TARGET "CppServer")

set (SOURCE_FILES "Server.cpp")
set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})

set(LOCAL_ALL_TARGETS ${ALL_TARGETS})
list(APPEND LOCAL_ALL_TARGETS ${THIS_TARGET})
set(ALL_TARGETS ${LOCAL_ALL_TARGETS} PARENT_SCOPE)