# TODO(thomthom): Configure for Mac:
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/third-party/slapi/win/headers")
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SLAPI_HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SLAPI_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(SketchUpAPICpp STATIC ${SLAPI_HEADERS} ${SLAPI_SOURCES})

# source_group(model REGULAR_EXPRESSION "/model/\.(hpp|cpp)")
