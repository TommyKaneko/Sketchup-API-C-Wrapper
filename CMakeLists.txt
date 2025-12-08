cmake_minimum_required(VERSION 3.17.0)
# Set policies before anything else
cmake_policy(VERSION 3.17)
set(CMAKE_POLICY_DEFAULT_CMP0175 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Allow older subprojects" FORCE)

# https://crascit.com/2015/03/28/enabling-cxx11-in-cmake/
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
# Specify the Apple Clang compiler
#set(CMAKE_C_COMPILER "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++")

# Set macOS deployment target and SDK
#set(CMAKE_OSX_DEPLOYMENT_TARGET "12.7" CACHE STRING "Minimum OS X deployment version")

# https://cmake.org/cmake/help/latest/guide/tutorial/index.html
project(SketchUpAPICpp)

# Look for TODO(Example) for where you need to provide your own input for your
# own project if you use this example as a template.

set(CMAKE_BUILD_TYPE Debug)

# Validate platform.
if(NOT (WIN32 OR APPLE))
  message(FATAL_ERROR "This project only works on Windows or macOS.")
endif()

# Depending on SketchUp 2020.2+ features.
# SketchUp 2020.2 supports macOS 10.13 and newer.
set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0" CACHE STRING "Minimum macOS deployment version")

# SketchUp 2022.0 supports macOS arm64 architecture. Ruby C Extensions built
# for older versions needs to adjust the OSX_ARCHITECTURE property to x86_64 only.
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "macOS archtectures to build")
#set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "macOS archtectures to build")

# Specify the list of directories to search for cmake modules.
# https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Hide symbols by default.
# https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Since SketchUp extensions are created to run against multiple SketchUp
# versions CRT dependency can be a pain if using dynamic linking.
# If the extension was build against the same CRT as SketchUp then it would be
# fine. But if the versions are different and the user doesn't have the
# extension's CRT installed the Ruby C Extension will fail to load.
# Different SketchUp versions using the same Ruby version might also have been
# built with different CRT, making it difficult to rely on SketchUp's CRT.
# To simplify this, build the Ruby C Extension with static CRT.
# Set here to apply as default for all targets.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set some Windows only properties.
if(WIN32)
  # Use the Unicode version of the Windows API.
  add_definitions(-DUNICODE -D_UNICODE)
  # Avoid global pollution from Windows API.
  add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
endif()

# TODO: Omitted warnings and testing setup later.
# # Link this 'library' to use the options specified in ProjectOptions.cmake.
# add_library(project_options INTERFACE)
# include(cmake/ProjectOptions.cmake)
# set_project_options(project_options)

# # Link this 'library' to use the warnings specified in ProjectWarnings.cmake.
# add_library(project_warnings INTERFACE)
# include(cmake/ProjectWarnings.cmake)
# set_project_warnings(project_warnings)

# # https://stackoverflow.com/a/50469013/486990
# # https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Testing-With-CTest
# include(CTest)
# enable_testing()

##############################################################################
# To tell this project where to find the SketchUp C API either:
# 1. `cmake -D "SketchUpAPI_SEARCH_DIR=<path_to/slapi>"
# 2. Set an "SketchUpAPI_SEARCH_DIR" environment variable
# 3. Extract the SDK zip files into the /third-party/slapi directory of this
#    project.
#
# In all these cases it's assumed that the directory contains (possibly)
# multiple versions of the C API, each in their own directory matching the
# original .zip files distrobuted.
#
# For example, the SketchUp 2020.0 SDK distrobuted the following files:
# * SDK_WIN_x64_2020-0-363.zip
# * SDK_Mac_2020-0-362.zip
#
# Extract these such that you have:
# * <path_to/slapi>/SDK_WIN_x64_2020-0-363/
# * <path_to/slapi>/SDK_Mac_2020-0-362/
#
# If you have multiple versions for Windows it might look like this:
# * <path_to/slapi>/SDK_WIN_x64_2020-0-363/
# * <path_to/slapi>/SDK_WIN_x64_2019-3-253/
# * <path_to/slapi>/SDK_WIN_x64_2019-2-222/
#
# The CMake module in this project will look for these spesific names and
# map them to their appropriate SketchUp versions when looking for the C API.
message(DEBUG "SketchUpAPI_SEARCH_DIR: ${SketchUpAPI_SEARCH_DIR}")
message(DEBUG "ENV SketchUpAPI_SEARCH_DIR: $ENV{SketchUpAPI_SEARCH_DIR}")
if(DEFINED SketchUpAPI_SEARCH_DIR OR DEFINED ENV{SketchUpAPI_SEARCH_DIR})
  message(DEBUG "Using pre-defined SLAPI path: ${SketchUpAPI_SEARCH_DIR}$ENV{SketchUpAPI_SEARCH_DIR}")
else()
  set(SketchUpAPI_SEARCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/slapi)
  message(DEBUG "Using project-defined SLAPI path: ${SketchUpAPI_SEARCH_DIR} $ENV{SketchUpAPI_SEARCH_DIR}")
endif()

# There are no aims to have submodules of this project, but included below for future.
# # Enable CMake to find the Ruby frameworks bundles in this project as a
# # submodule.
# list(PREPEND CMAKE_PREFIX_PATH
#   ${CMAKE_CURRENT_SOURCE_DIR}/third-party/ruby-c-extension-examples
# )

# Ensure we build static CRT for GoogleTest to match the rest of the project.
# https://github.com/google/googletest/blob/master/googletest/README.md
# https://crascit.com/2015/07/25/cmake-gtest/
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)

# Don't want to install GoogleTest.
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# Find the SketchUp C API SDK package:
#find_package(SketchUpAPI 2024.0 REQUIRED)
find_package(SketchUpAPI 2025.0 REQUIRED)


# Sub projects are not currently implemented, however, would be good to do tests in future.
# add_subdirectory(third-party/google-test)
# add_subdirectory(cext/image_lib)
# add_subdirectory(cext/ruby_extension)
# add_subdirectory(cext/tests/image_lib)

###################################################################
# Custom Code for SketchupCppAPI build
###################################################################

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

set (SLAPI_PATH "${_SketchUpAPI_LIBRARY_DIR}")
set(RUBY_PATH "${CPP_API_THIRD_PARTY_PATH}/ruby")
# TODO: Ruby version is set here, but a cleverer version is likely needed in future.
set(RUBY_VERSION "3.2")



################ OLD CODE BELOW FOR REFERENCE ####################



if ( MSVC )
  # TK: include path in case this library is built externally.  
  if (EXTERNALLY_BUILT)
    set(SLAPI_INCLUDE_PATH "${SketchUpAPI_INCLUDE_DIR}")
  else()
    set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/headers")
  endif()
  set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/headers")
  
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
  # TK: include path in case this library is built externally.  
  if (EXTERNALLY_BUILT)
    set(SLAPI_INCLUDE_PATH "${SketchUpAPI_INCLUDE_DIR}")
  else()
    set(SLAPI_INCLUDE_PATH "${SLAPI_PATH}/SketchUpAPI.framework/Headers")
  endif()
  
  set(SLAPI_LIB "${SLAPI_PATH}/SketchUpAPI.framework")
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

  # set(RUBY_INCLUDE_PATH "${RUBY_PATH}/lib/mac/${RUBY_VERSION}/Ruby.framework/Headers")
  # set(RUBY_LIB "${RUBY_PATH}/lib/mac/${RUBY_VERSION}/Ruby.framework")
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
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GoogleTest.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/SketchUpAPITests.cmake)
