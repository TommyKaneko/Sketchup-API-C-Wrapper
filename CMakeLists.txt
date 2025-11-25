cmake_minimum_required(VERSION 3.17.0)
# Set policies before anything else
cmake_policy(VERSION 3.17)
set(CMAKE_POLICY_DEFAULT_CMP0175 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Allow older subprojects" FORCE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Specify the Apple Clang compiler
#set(CMAKE_C_COMPILER "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++")

# Set macOS deployment target and SDK
#set(CMAKE_OSX_DEPLOYMENT_TARGET "12.7" CACHE STRING "Minimum OS X deployment version")

project(SketchUpAPICpp)

# See if the SketchUpAPI is externally built or not.
if (DEFINED SketchUpAPI_INCLUDE_DIR)
  set(EXTERNALLY_BUILT TRUE)
else()
  set(EXTERNALLY_BUILT FALSE)
endif()

# Define various paths:
set(CPP_API_BASENAME "SUAPI-Cpp")
set(CPP_API_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(CPP_API_SOURCE_PATH "${PROJECT_SOURCE_DIR}/src")
set(CPP_API_TESTS_PATH "${PROJECT_SOURCE_DIR}/tests")
set(CPP_API_CMAKE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CPP_API_THIRD_PARTY_PATH "${PROJECT_SOURCE_DIR}/third-party")

# set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# TODO(thomthom): Configure for Mac:
set(SLAPI_PATH "${CPP_API_THIRD_PARTY_PATH}/slapi")
set(RUBY_PATH "${CPP_API_THIRD_PARTY_PATH}/ruby")
# TODO: Ruby version is set here, but a cleverer version is likely needed in future.
set(RUBY_VERSION "2.5")

if ( MSVC )
  # # TK: include path in case this library is built externally.  
  # if (EXTERNALLY_BUILT)
  #   set(SLAPI_INCLUDE_PATH "${SketchUpAPI_INCLUDE_DIR}")
  # else()
  #   set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/win/headers")
  # endif()
  set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/win/headers")
  
  set(SLAPI_BINARIES_PATH "${SLAPI_PATH}/win/binaries")
  set(SLAPI_BINARIES_X64_PATH "${SLAPI_BINARIES_PATH}/sketchup/x64")
  # TODO(thomthom): Add library paths per platform, and try to add just "SketchUpAPI"
  #                 to `target_link_libraries`.
  set(SLAPI_LIB "${SLAPI_BINARIES_X64_PATH}/SketchUpAPI.lib")
  
  # configure Ruby library
  set(RUBY_INCLUDE_PATH "${RUBY_PATH}/include/${RUBY_VERSION}/win32_x64")
  set(RUBY_BINARIES_PATH "${RUBY_PATH}/lib/win32")
  set(RUBY_BINARIES_X64_PATH "${RUBY_BINARIES_PATH}")
  set(RUBY_LIB "${RUBY_BINARIES_X64_PATH}/x64-msvcrt-ruby250.lib")
else()
  # # TK: include path in case this library is built externally.  
  # if (EXTERNALLY_BUILT)
  #   set(SLAPI_INCLUDE_PATH "${SketchUpAPI_INCLUDE_DIR}")
  # else()
  #   set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/mac/SketchUpAPI.framework/Headers")
  # endif()
  
  ## New approach using find_package gets the Live C API from the application being run.
  # TODO: requires quite a bit of work on cmake to get this working properly.
  # find_package(SketchUpAPI 2024.0 REQUIRED)

  set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/mac/SketchUpAPI.framework/Headers")
  set(SLAPI_LIB "${SLAPI_PATH}/mac/SketchUpAPI.framework")
  set(SLAPI_SYMLINK_PATH "${SLAPI_INCLUDE_PATH}/SketchUpAPI")

  # Add symbolic links so that #include paths to the SketchUpAPI headers can read <SketchUpAPI/header.h>
  if (NOT EXISTS "${SLAPI_SYMLINK_PATH}")
      message(STATUS "Creating symbolic link: ${SLAPI_SYMLINK_PATH}")
      execute_process(
          COMMAND ln -s "${SLAPI_INCLUDE_PATH}" "${SLAPI_SYMLINK_PATH}"
          RESULT_VARIABLE symlink_result
      )
      if (NOT symlink_result EQUAL 0)
          message(FATAL_ERROR "Failed to create symbolic link: ${SLAPI_SYMLINK_PATH}")
      endif()
  endif()

  set(RUBY_INCLUDE_PATH "${RUBY_PATH}/lib/mac/${RUBY_VERSION}/Ruby.framework/Headers")
  set(RUBY_LIB "${RUBY_PATH}/lib/mac/${RUBY_VERSION}/Ruby.framework")
endif()

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
#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GoogleTest.cmake)
#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SketchUpAPITests.cmake)
