include_directories("${PROJECT_SOURCE_DIR}/tests")
include_directories("${PROJECT_SOURCE_DIR}/third-party/googletest/googletest/include")

file(GLOB_RECURSE TESTS_HEADERS ${PROJECT_SOURCE_DIR}/tests/*.hpp)
file(GLOB_RECURSE TESTS_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

add_executable(SketchUpAPITests ${TESTS_HEADERS} ${TESTS_SOURCES})

# TODO(thomthom): Add library paths per platform, and try to add just "SketchUpAPI"
#                 to `target_link_libraries`.
set(SLAPI_LIB "${PROJECT_SOURCE_DIR}/third-party/slapi/win/binaries/sketchup/x64/SketchUpAPI.lib")

target_link_libraries(SketchUpAPITests GoogleTest SketchUpAPICpp ${SLAPI_LIB})

# TODO(thomthom): Set up per-platform post-build commands.
add_custom_command(TARGET SketchUpAPITests POST_BUILD
  COMMAND xcopy \"${PROJECT_SOURCE_DIR}\\third-party\\slapi\\win\\binaries\\sketchup\\x64\\SketchUpAPI.dll\" $(OutputPath) /D /Y
  COMMAND xcopy \"${PROJECT_SOURCE_DIR}\\third-party\\slapi\\win\\binaries\\sketchup\\x64\\SketchUpCommonPreferences.dll\" $(OutputPath) /D /Y
)
