# Таргет с контроллером. Контроллер - самый главный тут. 
# Запускает и управляет всеми процессами.
set (THIS_TARGET "CController")

set (SOURCE_FILES 
  "Controller.c"
  "Impl/DoProcess.c"
)
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

set(LOCAL_ALL_TARGETS ${ALL_TARGETS})
list(APPEND LOCAL_ALL_TARGETS ${THIS_TARGET})
set(ALL_TARGETS ${LOCAL_ALL_TARGETS} PARENT_SCOPE)