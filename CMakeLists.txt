cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 14)

project (SketchUpApiCppWrapper)

# Define various paths:

set(CPP_API_BASENAME "SUAPI-CppWrapper")
set(CPP_API_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(CPP_API_SOURCE_PATH "${PROJECT_SOURCE_DIR}/src")
set(CPP_API_TESTS_PATH "${PROJECT_SOURCE_DIR}/tests")

set(CPP_API_THIRD_PARTY_PATH "${PROJECT_SOURCE_DIR}/third-party")

# TODO(thomthom): Configure for Mac:
set(SLAPI_PATH "${CPP_API_THIRD_PARTY_PATH}/slapi")
set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/win/headers")
set(SLAPI_BINARIES_PATH "${SLAPI_PATH}/win/binaries")
set(SLAPI_BINARIES_X64_PATH "${SLAPI_BINARIES_PATH}/sketchup/x64")
# TODO(thomthom): Add library paths per platform, and try to add just "SketchUpAPI"
#                 to `target_link_libraries`.
set(SLAPI_LIB "${SLAPI_BINARIES_X64_PATH}/SketchUpAPI.lib")

set(GOOGLETEST_PATH "${CPP_API_THIRD_PARTY_PATH}/googletest/googletest")
set(GOOGLETEST_INCLUDE_PATH "${GOOGLETEST_PATH}/include")


# GoogleTest 1.8 produce a warning that must be silenced. The fix is yet to be
# pushed to the GoogleTest stable branch.
#   warning C4996: 'std::tr1': warning STL4002: The non-Standard std::tr1
#   namespace and TR1-only machinery are deprecated and will be REMOVED. You can
#   define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING to acknowledge that you
#   have received this warning.
add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

# Avoid conflicting macros from Win32 API.
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX)


include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SketchUpAPICpp.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GoogleTest.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SketchUpAPITests.cmake)
