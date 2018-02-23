include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/third-party/googletest/googletest/include")
include_directories("${PROJECT_SOURCE_DIR}/third-party/googletest/googletest")

add_library(GoogleTest STATIC third-party/googletest/googletest/src/gtest-all.cc)
