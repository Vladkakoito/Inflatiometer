set (THIS_TARGET "IniParser")

add_library(${THIS_TARGET} STATIC 
  "Impl/IniParser.c"
)

target_link_libraries(${THIS_TARGET} Ini)
target_include_directories(${THIS_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/C")
