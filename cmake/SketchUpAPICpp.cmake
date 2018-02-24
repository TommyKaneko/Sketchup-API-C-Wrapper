# TODO(thomthom): Configure for Mac:
include_directories(SYSTEM "${SLAPI_INCLUDE_PATH}")
include_directories("${CPP_API_INCLUDE_PATH}")

file(GLOB_RECURSE CPP_API_HEADERS ${CPP_API_INCLUDE_PATH}/*.hpp)
file(GLOB_RECURSE CPP_API_SOURCES ${CPP_API_SOURCE_PATH}/*.cpp)

add_library(SketchUpAPICpp STATIC ${CPP_API_HEADERS} ${CPP_API_SOURCES})

source_group(
  TREE "${CPP_API_INCLUDE_PATH}/${CPP_API_BASENAME}"
  PREFIX "Header Files"
  FILES ${CPP_API_HEADERS}
)
source_group(
  TREE "${CPP_API_SOURCE_PATH}/${CPP_API_BASENAME}"
  PREFIX "Source Files"
  FILES ${CPP_API_SOURCES}
)
