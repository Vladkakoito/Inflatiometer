set (THIS_TARGET "Logger")

add_library(${THIS_TARGET} STATIC 
  "Impl/Logger.c"
  "Impl/IniParser.c"
)

target_link_libraries(${THIS_TARGET} Ini)
target_include_directories(${THIS_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/C")
