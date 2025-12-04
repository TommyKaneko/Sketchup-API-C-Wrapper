# https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html
# https://cmake.org/cmake/help/latest/manual/cmake-developer.7.html#manual:cmake-developer(7)

# ${SketchUpAPI_DIR}
# ${SketchUpAPI_SEARCH_DIR}
# $ENV{SketchUpAPI_DIR}
# $ENV{SketchUpAPI_SEARCH_DIR}

message(DEBUG "FindSketchUpAPI")
# TODO: More SDKs to be set up here
set(_SketchUpAPI_2025_0 #
  2025.0
  SDK_WIN_x64_2025-0-634
  SDK_Mac_2025-0-633
)
set(_SketchUpAPI_2024_0 # The version is actualy 2024.0.2
  2024.0
  SDK_WIN_x64_2024-0-594
  SDK_Mac_2024-0-595
)
# set(_SketchUpAPI_2023_2 #
#   2023.2
#   SDK_WIN_x64_2023-2-101
#   SDK_Mac_2023-2-102
# )
# set(_SketchUpAPI_2022_0 #
#   2022.0
#   SDK_WIN_x64_2022-0-354
#   SDK_Mac_2022-0-353
# )
# set(_SketchUpAPI_2021_1 #
#   2021.1
#   SDK_WIN_x64_2021-1-332
#   SDK_Mac_2021-1-331
# )
# set(_SketchUpAPI_2020_2 #
#   2020.2
#   SDK_WIN_x64_2020-2-172
#   SDK_Mac_2020-2-171
# )


set(_SketchUpAPIs
  ${_SketchUpAPI_2025_0}
  ${_SketchUpAPI_2024_0}
  # ${_SketchUpAPI_2023_2}
  # ${_SketchUpAPI_2022_0}
  # ${_SketchUpAPI_2021_1}
  # ${_SketchUpAPI_2020_2}
)

set(_num_versions 1) # UPDATE: Should match items in `_SketchUpAPIs`
set(_version_size 3) # Size of the _SketchUpAPI_* vars.

LIST(LENGTH _SketchUpAPIs _length)
MATH(EXPR _range_max "(${_version_size}*${_num_versions})-1")

message(DEBUG "SketchUpAPI_DIR: ${SketchUpAPI_DIR}")
message(DEBUG "SketchUpAPI_DIR (ENV): $ENV{SketchUpAPI_DIR}")

if(DEFINED SketchUpAPI_DIR OR DEFINED ENV{SketchUpAPI_DIR})
  # If an explicit path has been set, use that.
  if (DEFINED SketchUpAPI_DIR)
    get_filename_component(_SketchUpAPI_DIR ${SketchUpAPI_DIR} REALPATH)
  else()
    get_filename_component(_SketchUpAPI_DIR $ENV{SketchUpAPI_DIR} REALPATH)
  endif()
  message(STATUS "Using explicit SketchUpAPI directory path: ${_SketchUpAPI_DIR}")
  # Don't know how to infer a version if not based on the official filenames.
  message(STATUS "Unknown SketchUpAPI package version")

else() # Otherwise, search for a location.

  foreach(_i RANGE 0 ${_range_max} ${_version_size})
    list(SUBLIST _SketchUpAPIs ${_i} ${_version_size} _SketchUpAPI)
    message(DEBUG "===============================")
    message(DEBUG "i: ${_i}, length: ${_length}, range_max: ${_range_max}")
    message(DEBUG "_SketchUpAPI: ${_SketchUpAPI}")

    list(GET _SketchUpAPI 0 _SketchUpAPI_VERSION)
    list(GET _SketchUpAPI 1 _SketchUpAPI_WIN_DIR)
    list(GET _SketchUpAPI 2 _SketchUpAPI_MAC_DIR)

    message(DEBUG "_SketchUpAPI_VERSION: ${_SketchUpAPI_VERSION}")

    if(WIN32)
      set(_SketchUpAPI_BASENAME_DIR ${_SketchUpAPI_WIN_DIR})
    elseif(APPLE)
      set(_SketchUpAPI_BASENAME_DIR ${_SketchUpAPI_MAC_DIR})
    else()
      message(FATAL_ERROR "SketchUpAPI is only available on Win/Mac")
    endif()
    message(DEBUG "_SketchUpAPI_BASENAME_DIR: ${_SketchUpAPI_BASENAME_DIR}")

    # TODO: Rename to SketchUpAPI_SEARCH_PATH?
    message(DEBUG "SketchUpAPI_SEARCH_DIR: ${SketchUpAPI_SEARCH_DIR}")
    message(DEBUG "ENV SketchUpAPI_SEARCH_DIR: $ENV{SketchUpAPI_SEARCH_DIR}")
    message(DEBUG "_SketchUpAPI_BASENAME_DIR: ${_SketchUpAPI_BASENAME_DIR}")

    find_file(_SketchUpAPI_DIR
      NAMES
        ${_SketchUpAPI_BASENAME_DIR}
      HINTS
        ${SketchUpAPI_SEARCH_DIR}
        $ENV{SketchUpAPI_SEARCH_DIR}
    )
    message(DEBUG "_SketchUpAPI_DIR: ${_SketchUpAPI_DIR}")

    if(NOT IS_DIRECTORY ${_SketchUpAPI_DIR})
      message(DEBUG "_SketchUpAPI_DIR: NOT IS_DIRECTORY")
      continue()
    endif()

    message(DEBUG "SketchUpAPI_FIND_VERSION_EXACT: ${SketchUpAPI_FIND_VERSION_EXACT}")
    message(DEBUG "SketchUpAPI_FIND_VERSION: ${SketchUpAPI_FIND_VERSION}")
    if(SketchUpAPI_FIND_VERSION_EXACT)
      if(NOT ${_SketchUpAPI_VERSION} VERSION_EQUAL ${SketchUpAPI_FIND_VERSION})
        message(DEBUG "NOT SketchUpAPI_FIND_VERSION VERSION_EQUAL")
        continue()
      endif()
    else()
      if(NOT ${_SketchUpAPI_VERSION} VERSION_GREATER_EQUAL ${SketchUpAPI_FIND_VERSION})
        message(DEBUG "NOT SketchUpAPI_FIND_VERSION VERSION_GREATER_EQUAL")
        continue()
      endif()
    endif()

    set(SketchUpAPI_DIR ${_SketchUpAPI_DIR})
    set(SketchUpAPI_VERSION ${_SketchUpAPI_VERSION})
    message(DEBUG "Found SketchUpAPI: ${SketchUpAPI_VERSION} (SketchUpAPI_VERSION) ")
    break()

  endforeach()

endif()

if(WIN32)
  set(_SketchUpAPI_LIBRARY_NAME "SketchUpAPI") # Standalone C API
  set(_SketchUpAPI_LIVE_LIBRARY_NAME "sketchup") # Live C API
  set(_SketchUpAPI_LIBRARY_DIR ${_SketchUpAPI_DIR}/binaries/sketchup/x64)
  set(_SketchUpAPI_INCLUDE_DIR ${_SketchUpAPI_DIR}/headers)
elseif(APPLE)
  set(_SketchUpAPI_LIBRARY_NAME "SketchUpAPI") # Standalone C API
  set(_SketchUpAPI_LIVE_LIBRARY_NAME "SketchUpAPI") # Live C API (Don't link!)
  set(_SketchUpAPI_LIBRARY_DIR ${_SketchUpAPI_DIR})
  set(_SketchUpAPI_INCLUDE_DIR ${_SketchUpAPI_DIR})
endif()
set(_SketchUpAPI_COMMON_PREFERENCES_LIBRARY_NAME "SketchUpCommonPreferences")

message(DEBUG "_SketchUpAPI_LIBRARY_DIR: ${_SketchUpAPI_LIBRARY_DIR}")
message(DEBUG "_SketchUpAPI_INCLUDE_DIR: ${_SketchUpAPI_INCLUDE_DIR}")

find_path(SketchUpAPI_INCLUDE_DIR "SketchUpAPI/sketchup.h"
  HINTS ${_SketchUpAPI_INCLUDE_DIR}
)
# Standalone C API
find_library(SketchUpAPI_LIBRARY ${_SketchUpAPI_LIBRARY_NAME}
  HINTS ${_SketchUpAPI_LIBRARY_DIR}
)
# Live C API
find_library(SketchUpAPI_LIVE_LIBRARY ${_SketchUpAPI_LIVE_LIBRARY_NAME}
  HINTS ${_SketchUpAPI_LIBRARY_DIR}
)

if(APPLE)
  # Allow the bundle loader to be explicitly set. This is needed if
  # SketchUpAPI_DIR is set since it is then not possible to infer package
  # version.
  message(DEBUG "SketchUpAPI_BUNDLE_LOADER: ${SketchUpAPI_BUNDLE_LOADER}")
  message(DEBUG "SketchUpAPI_BUNDLE_LOADER (ENV): $ENV{SketchUpAPI_BUNDLE_LOADER}")

  if (DEFINED ENV{SketchUpAPI_BUNDLE_LOADER})
    set(SketchUpAPI_BUNDLE_LOADER $ENV{SketchUpAPI_BUNDLE_LOADER})
  endif()

  if(SketchUpAPI_BUNDLE_LOADER)
    # If an explicit path has been set, use that.
    message(STATUS "Using explicit SketchUpAPI bundle loader: ${SketchUpAPI_BUNDLE_LOADER}")
    if(NOT EXISTS ${SketchUpAPI_BUNDLE_LOADER})
      message(FATAL_ERROR "Unable to find bundle loader: ${SketchUpAPI_BUNDLE_LOADER}")
    endif()

  else()
    # Pick path based on requested version.
    string(REPLACE "." ";" _SketchUpAPI_VERSION_COMPONENTS "${SketchUpAPI_VERSION}")
    message(DEBUG "_SketchUpAPI_VERSION: ${_SketchUpAPI_VERSION}")
    message(DEBUG "SketchUpAPI_VERSION: ${SketchUpAPI_VERSION}")
    message(DEBUG "_SketchUpAPI_VERSION_COMPONENTS: ${_SketchUpAPI_VERSION_COMPONENTS}")
    list(GET _SketchUpAPI_VERSION_COMPONENTS 0 _SketchUpAPI_VERSION_MAJOR)
    list(GET _SketchUpAPI_VERSION_COMPONENTS 1 _SketchUpAPI_VERSION_MINOR)
    set(_SketchUpAPI_PATH_SUFFIX "SketchUp ${_SketchUpAPI_VERSION_MAJOR}")
    message(DEBUG "_SketchUpAPI_PATH_SUFFIX: ${_SketchUpAPI_PATH_SUFFIX}")
    unset(_SketchUpAPI_APP CACHE)
    find_program(SketchUpAPI_BUNDLE_LOADER "SketchUp" PATH_SUFFIXES ${_SketchUpAPI_PATH_SUFFIX})
    message(DEBUG "SketchUpAPI_BUNDLE_LOADER: ${SketchUpAPI_BUNDLE_LOADER}")
    # TODO: Search paths for multiple SketchUp application versions
    #       independent of the requested package version? Equal or newer?
    # TODO: Make this part of REQUIRED_VARS on macOS?
    if(NOT EXISTS ${SketchUpAPI_BUNDLE_LOADER})
      message(FATAL_ERROR "Unable to find bundle loader: ${SketchUpAPI_BUNDLE_LOADER}")
    endif()
  endif()
endif()

mark_as_advanced(
  SketchUpAPI_INCLUDE_DIR
  SketchUpAPI_LIBRARY
  SketchUpAPI_LIVE_LIBRARY
  SketchUpAPI_BUNDLE_LOADER
)

message(DEBUG "SketchUpAPI_LIBRARY: ${SketchUpAPI_LIBRARY}")
message(DEBUG "SketchUpAPI_LIVE_LIBRARY: ${SketchUpAPI_LIVE_LIBRARY}")
message(DEBUG "SketchUpAPI_INCLUDE_DIR: ${SketchUpAPI_INCLUDE_DIR}")
message(DEBUG "SketchUpAPI_COMMON_PREFERENCES_LIBRARY: ${SketchUpAPI_COMMON_PREFERENCES_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SketchUpAPI
  REQUIRED_VARS
    SketchUpAPI_LIBRARY
    SketchUpAPI_LIVE_LIBRARY
    SketchUpAPI_INCLUDE_DIR
  VERSION_VAR
    SketchUpAPI_VERSION)

if(SketchUpAPI_FOUND)
  set(SketchUpAPI_LIBRARIES ${SketchUpAPI_LIBRARY} ${SketchUpAPI_COMMON_PREFERENCES_LIBRARY})
  set(SketchUpAPI_LIVE_LIBRARIES ${SketchUpAPI_LIVE_LIBRARY} ${SketchUpAPI_COMMON_PREFERENCES_LIBRARY})
  set(SketchUpAPI_INCLUDE_DIRS ${SketchUpAPI_INCLUDE_DIR})
  
  string(REPLACE "." ";" _SketchUpAPI_VERSION_COMPONENTS ${SketchUpAPI_VERSION})
  list(GET _SketchUpAPI_VERSION_COMPONENTS 0 _SketchUpAPI_VERSION_MAJOR)
  list(GET _SketchUpAPI_VERSION_COMPONENTS 1 _SketchUpAPI_VERSION_MINOR)
  add_compile_definitions(SketchUpAPI_VERSION_MAJOR=${_SketchUpAPI_VERSION_MAJOR})
  add_compile_definitions(SketchUpAPI_VERSION_MINOR=${_SketchUpAPI_VERSION_MINOR})
  message(STATUS "Compile definition: SketchUpAPI_VERSION_MAJOR: ${_SketchUpAPI_VERSION_MAJOR}")
  message(STATUS "Compile definition: SketchUpAPI_VERSION_MINOR: ${_SketchUpAPI_VERSION_MINOR}")


  # These DLLs needs to be loaded by the consumer of the standalone SketchUp C API.
  if(WIN32)
    set(SketchUpAPI_BINARIES
      ${_SketchUpAPI_LIBRARY_DIR}/${_SketchUpAPI_LIBRARY_NAME}.dll
      ${_SketchUpAPI_LIBRARY_DIR}/${_SketchUpAPI_COMMON_PREFERENCES_LIBRARY_NAME}.dll
    )
  endif()
endif()

# https://cmake.org/cmake/help/latest/manual/cmake-developer.7.html#a-sample-find-module
# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#imported-targets
# Professional CMake - Chapter 5.2.
if(SketchUpAPI_FOUND AND NOT TARGET SketchUp::SketchUpAPI)
  if(APPLE)
    set(SketchUpAPI_LIBRARY "${SketchUpAPI_LIBRARY}/${_SketchUpAPI_LIBRARY_NAME}")
  endif()

  # Standalone C API
  add_library(SketchUp::SketchUpAPI SHARED IMPORTED)
  set_target_properties(SketchUp::SketchUpAPI PROPERTIES
    IMPORTED_LOCATION "${SketchUpAPI_LIBRARY}"
    IMPORTED_IMPLIB "${SketchUpAPI_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SketchUpAPI_INCLUDE_DIR}"
  )

  # Live C API
  # TODO: Can UNKNOWN be omitted?
  # TODO: Mac should not link to the SketchUpAPI framework - only use include dir.
  if(WIN32)
    # On Windows we must link to sketchup.lib to use the Live C API.
    add_library(SketchUp::SketchUpLiveAPI UNKNOWN IMPORTED)
    set_target_properties(SketchUp::SketchUpLiveAPI PROPERTIES
      IMPORTED_LOCATION "${SketchUpAPI_LIVE_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${SketchUpAPI_INCLUDE_DIR}"
    )
  elseif(APPLE)
    # TODO: Can this be SHARED?
    # TODO: Can INTERFACE be used on Windows as well?
    #       Just omit IMPORTED_LOCATION on mac.
    add_library(SketchUp::SketchUpLiveAPI INTERFACE IMPORTED)
    set_target_properties(SketchUp::SketchUpLiveAPI PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${SketchUpAPI_INCLUDE_DIR}"
    )
    # Since we aren't linking to the SketchUpAPI.framework we must set the
    # bundle loader to a version of SketchUp.
    # NOTE: This doesn't lock the compiled binary to this SketchUp version, it's
    #       only needed during linking. The only requirement is that the version
    #       used support the SLAPI features used.
    target_link_options(SketchUp::SketchUpLiveAPI INTERFACE
      LINKER:-bundle -bundle_loader ${SketchUpAPI_BUNDLE_LOADER}
    )
    # TODO: Check @executable_path etc...
    # example.bundle:
    # 	@executable_path/../Frameworks/Ruby.framework/Versions/Current/Ruby (compatibility version 2.5.0, current version 2.5.1)
    # 	/usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 400.9.4)
    # 	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.250.1)
  endif()
endif()
