set (THIS_TARGET "CDatabaseManager")

set (SOURCE_FILES 
  "DatabaseManager.c"
  "Impl/ContentDescription/Json/NodesWalker.c"
  "Impl/Manager.c"
)

if (DEFINED WITH_POSTGRES_DB)
  set (SOURCE_FILES ${SOURCE_FILES} 
    "Impl/Postgres/Commands.c"
    "Impl/Postgres/Connector.c"
    "Impl/Postgres/PostgresDB.c"
  )
endif()

set (HEADER_FILES "")

add_executable(${THIS_TARGET} ${SOURCE_FILES} ${HEADER_FILES})

if (DEFINED WITH_POSTGRES_DB)
  target_compile_definitions(${THIS_TARGET} PRIVATE WITH_POSTGRES_DB)
  target_include_directories(${THIS_TARGET} PRIVATE ${POSTGRES_INCLUDE_DIR})
  target_link_directories(${THIS_TARGET} PRIVATE ${POSTGRES_LINK_DIR})
  message (STATUS ${POSTGRES_INCLUDE_DIR})
  message (STATUS ${POSTGRES_LINK_DIR})
endif()

target_link_libraries(${THIS_TARGET} 
  IniParser
  Logger
  Functions
  cjson
  pq
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