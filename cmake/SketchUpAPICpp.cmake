# TODO(thomthom): Configure for Mac:
include_directories(SYSTEM "${SLAPI_INCLUDE_PATH}")
include_directories("${CPP_API_INCLUDE_PATH}")

file(GLOB_RECURSE CPP_API_HEADERS ${CPP_API_INCLUDE_PATH}/*.hpp)
file(GLOB_RECURSE CPP_API_SOURCES ${CPP_API_SOURCE_PATH}/*.cpp)

add_library(SketchUpAPICpp STATIC ${CPP_API_HEADERS} ${CPP_API_SOURCES})
target_link_libraries(SketchUpAPICpp LINK_PUBLIC SLAPI_LIB)

# https://stackoverflow.com/a/14235055/486990
if ( MSVC )
  # TODO(thomthom): Enable /W4 when the warnings it trigger is addressed.
  # target_compile_options(SketchUpAPICpp PRIVATE "/W4")
  target_compile_options(SketchUpAPICpp PRIVATE "/W3")
  target_compile_options(SketchUpAPICpp PRIVATE "/WX")
  target_compile_options(SketchUpAPICpp PRIVATE "/MP")
else()
  # target_compile_options(SketchUpAPICpp PRIVATE "-Wall")
  target_compile_options(SketchUpAPICpp PRIVATE "-Wno-missing-braces")
  target_compile_options(SketchUpAPICpp PRIVATE "-Werror")
endif()

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
