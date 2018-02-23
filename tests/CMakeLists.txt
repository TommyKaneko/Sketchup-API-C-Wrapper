cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}/tests")
include_directories("${PROJECT_SOURCE_DIR}/third-party/googletest/googletest/include")

add_executable(SketchUpAPITests tests/SketchUpAPITests.cpp)
