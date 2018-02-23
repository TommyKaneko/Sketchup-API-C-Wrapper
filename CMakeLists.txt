cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")

project (SketchUpApi)

# set (SOLUTION_PATH PROJECT_SOURCE_DIR)

# GoogleTest 1.8 produce a warning that must be silenced. The fix is yet to be
# pushed to the GoogleTest stable branch.
#   warning C4996: 'std::tr1': warning STL4002: The non-Standard std::tr1
#   namespace and TR1-only machinery are deprecated and will be REMOVED. You can
#   define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING to acknowledge that you
#   have received this warning.
add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeGoogleTest.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/SketchUpAPITests.cmake)
