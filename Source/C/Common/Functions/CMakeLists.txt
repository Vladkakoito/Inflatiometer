set (THIS_TARGET "Functions")

add_library(${THIS_TARGET} STATIC 
  "Impl/Functions.c"
)

target_include_directories(${THIS_TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/C")