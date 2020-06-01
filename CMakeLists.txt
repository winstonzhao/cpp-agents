cmake_minimum_required(VERSION 3.1)
project(cpp-agents)

# Set build type to RELEASE by default:
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type provided, default to RELEASE.")
  set(CMAKE_BUILD_TYPE "RELEASE")
endif()

# Options can be turned on and off by providing cmake with a -DOPTIONNAME=ON (or OFF) argument.
# For example, to turn off benchmarks, build a shared library, and use sanitize flags in a DEBUG build:
#    cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DAGENTS_BENCH=OFF -DBENCH_STATIC=OFF -DAGENTS_SANITIZE=ON
option(AGENTS_TEST      "Build tests" ON)
option(AGENTS_BENCH     "Build benchmarks" ON)
option(AGENTS_STATIC    "Builds a static library instead of a shared one" OFF)
option(AGENTS_SANITIZE  "Adds sanitive flags" OFF)

if(AGENTS_STATIC)
  set(AGENTS_LIB_NAME agents_static)
  set(AGENTS_LIB_TYPE STATIC)
else()
  set(AGENTS_LIB_NAME agents)
  set(AGENTS_LIB_TYPE SHARED)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Threads REQUIRED)
find_package(Boost)
find_package(Math)

include_directories(. include ${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11) # cmake 3.1 supports '98', '11' or '14'. cmake 3.8 or later support '17'.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(AGENTS_SANITIZE)
  set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize=undefined ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(src)

if(AGENTS_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

if(AGENTS_BENCH)
  add_subdirectory(bench)
endif()

message(STATUS "")
message(STATUS "BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID          : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Library name         : ${AGENTS_LIB_NAME}")
message(STATUS "  Library type         : ${AGENTS_LIB_TYPE}")
message(STATUS "  Build tests          : ${AGENTS_TEST}")
message(STATUS "  Build benchmarks     : ${AGENTS_BENCH}")
message(STATUS "  Sanitize flags       : ${AGENTS_SANITIZE}")
message(STATUS "  Boost include dirs   : ${Boost_INCLUDE_DIRS}")
message(STATUS "")
