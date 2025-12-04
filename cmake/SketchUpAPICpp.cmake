message(DEBUG PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR})
message(DEBUG SketchUpAPI_INCLUDE_DIR: ${SketchUpAPI_INCLUDE_DIR})

# TODO(thomthom): Configure for Mac:
include_directories(SYSTEM "${SLAPI_INCLUDE_PATH}")
#include_directories(SYSTEM "${RUBY_INCLUDE_PATH}")
include_directories("${CPP_API_INCLUDE_PATH}")

file(GLOB_RECURSE CPP_API_HEADERS ${CPP_API_INCLUDE_PATH}/*.hpp)
file(GLOB_RECURSE CPP_API_SOURCES ${CPP_API_SOURCE_PATH}/*.cpp)

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

### Old method of adding library - it was static only, and we wish to allow linking against the Live C API
# add_library(SketchUpAPICpp STATIC ${CPP_API_HEADERS} ${CPP_API_SOURCES})

### New method of adding library below
# Set static library name here:
set(LIBRARY_BASE_NAME SketchUpAPICpp)
# This is the library name for the library linking against the Live C API.
set(LIBRARY_NAME ${LIBRARY_BASE_NAME}_live)
# This is a static lib used for unit tests - which must link against the
# standalone version of the C API since they don't run under SketchUp.
set(TEST_LIBRARY_NAME ${LIBRARY_BASE_NAME}_test_static)

# Create a library for Live C API usage:
add_library(${LIBRARY_NAME} STATIC ${CPP_API_HEADERS} ${CPP_API_SOURCES})
# Create a library for Standalone C API usage (tests etc.):
add_library(${TEST_LIBRARY_NAME} STATIC ${CPP_API_HEADERS} ${CPP_API_SOURCES})

# Link public libraries:
target_link_libraries(${LIBRARY_NAME} PUBLIC
  SketchUp::SketchUpLiveAPI
)
target_link_libraries(${TEST_LIBRARY_NAME} PUBLIC
  SketchUp::SketchUpAPI
)

# The projects include paths:
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${TEST_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})



# # Include headers for the library:
# target_include_directories(SketchUpAPICpp PUBLIC
#   ${CMAKE_CURRENT_SOURECE_DIR}
#   ${SLAPI_INCLUDE_PATH}
# )

# # Link the SketchUpAPI framework on macOS
# if (APPLE)
#     # TK: the framework is not being found - link it here with the necessary flag
#     target_link_options(SketchUpAPICpp INTERFACE "-F${SLAPI_PATH}/mac")
#     message(DEBUG "SketchUpAPI path for SketchUpAPICpp: ${SLAPI_PATH}/mac")
#     # Add the framework search path
#     target_link_directories(SketchUpAPICpp PUBLIC "${SLAPI_PATH}/mac")
#     # Link the SketchUpAPI framework
#     target_link_libraries(SketchUpAPICpp "-framework SketchUpAPI")
# else()
#     # Windows-specific linking
#     target_link_libraries(SketchUpAPICpp "${SLAPI_LIB}")
# endif()

# # https://stackoverflow.com/a/14235055/486990
# if ( MSVC )
#   # TODO(thomthom): Enable /W4 when the warnings it trigger is addressed.
#   # target_compile_options(SketchUpAPICpp PRIVATE "/W4")
#   target_compile_options(SketchUpAPICpp PRIVATE "/W3")
#   target_compile_options(SketchUpAPICpp PRIVATE "/WX")
#   target_compile_options(SketchUpAPICpp PRIVATE "/MP")
# else()
#   # target_compile_options(SketchUpAPICpp PRIVATE "-Wall")
#   target_compile_options(SketchUpAPICpp PRIVATE "-Wno-missing-braces")
#   if (EXTERNALLY_BUILT)
#     target_compile_options(SketchUpAPICpp PRIVATE "-Wno-error") # TK: suppress warnings when built externally
#   else()
#     #target_compile_options(SketchUpAPICpp PRIVATE "-Werror")
#     target_compile_options(SketchUpAPICpp PRIVATE "-Wno-error") # TODO: remove this flag later.
#   endif()
# endif()
