message(STATUS "================= SketchUp API C++ Wrapper Tests =================")
# Where we keep the test models
set(CPP_API_TESTS_MODELS_PATH "${PROJECT_SOURCE_DIR}/models")
# Where we keep the test output models
set(CPP_API_TESTS_MODELS_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/test_model_output")
# Tell the test source code where to find the models
configure_file (
  "${CPP_API_TESTS_PATH}/ModelPath.h.in"
  "${PROJECT_BINARY_DIR}/ModelPath.h"
)

include_directories("${CPP_API_TESTS_PATH}")
include_directories("${GOOGLETEST_INCLUDE_PATH}")
include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE TESTS_HEADERS ${CPP_API_TESTS_PATH}/*.hpp)
file(GLOB_RECURSE TESTS_SOURCES ${CPP_API_TESTS_PATH}/*.cpp)

add_executable(SketchUpAPITests ${TESTS_HEADERS} ${TESTS_SOURCES})

target_link_libraries(SketchUpAPITests GoogleTest ${TEST_LIBRARY_NAME} ${SLAPI_LIB})
message(STATUS "Linking to Library: ${SLAPI_LIB}")

set_target_properties(${SketchUpAPITests} PROPERTIES COMPILE_FLAGS "-Wall")

# Address Sanitizer option (Comment out if not needed)
target_compile_options(SketchUpAPITests
  PRIVATE -fsanitize=address
  #PRIVATE -g -O1 -fno-omit-frame-pointer
  )
target_link_options(SketchUpAPITests PRIVATE -fsanitize=address)
message(STATUS "Address Sanitizer enabled for SketchUpAPITests - Make sure this is disabled for production builds!")

source_group(
  "Tests"
  REGULAR_EXPRESSION "${CPP_API_TESTS_PATH}/[^\//]+Tests.cpp"
)
# Force the CPP file that define `main` to not appear in the Tests list.
source_group(
  "Source Files"
  FILES "${CPP_API_TESTS_PATH}/SketchUpAPITests.cpp"
)

# TODO(thomthom): Set up per-platform post-build commands.
if ( MSVC )
  add_custom_command(TARGET SketchUpAPITests POST_BUILD
    # xcopy seem to require at least one \ slash in the source path, otherwise it
    # appear to fail.
    COMMAND xcopy \"${SLAPI_BINARIES_PATH}\\sketchup\\x64\\SketchUpAPI.dll\" $(OutputPath) /D /Y
    COMMAND xcopy \"${SLAPI_BINARIES_PATH}\\sketchup\\x64\\SketchUpCommonPreferences.dll\" $(OutputPath) /D /Y
  )
else()
endif()

