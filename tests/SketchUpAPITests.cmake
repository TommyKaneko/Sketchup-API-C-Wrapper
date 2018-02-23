include_directories("${PROJECT_SOURCE_DIR}/tests")
include_directories("${PROJECT_SOURCE_DIR}/third-party/googletest/googletest/include")

add_executable(SketchUpAPITests tests/SketchUpAPITests.cpp)

target_link_libraries(SketchUpAPITests GoogleTest)
