cmake_minimum_required(VERSION 3.5.0)
project(teuthid VERSION 0.1.0)
set(teuthid_library_soversion 0)
set(teuthid_library_version 
${teuthid_library_soversion}.${teuthid_VERSION_MAJOR}.${teuthid_VERSION_MINOR})

set(teuthid_CMAKE_DIR "${teuthid_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${teuthid_CMAKE_DIR} ${CMAKE_MODULE_PATH})
include(CheckOutOfSourceBuild)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" _cmake_build_type)
if (NOT _cmake_build_type STREQUAL "debug"
      AND NOT _cmake_build_type STREQUAL "release"
      AND NOT _cmake_build_type STREQUAL "relwithdebinfo"
      AND NOT _cmake_build_type STREQUAL "minsizerel")
  message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH 
    "default install path" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
SET(CMAKE_COLOR_MAKEFILE ON)

set(teuthid_INCLUDE_PATH ${teuthid_SOURCE_DIR}/include)
set(teuthid_INCLUDE_DIR ${teuthid_SOURCE_DIR}/include/teuthid)
set(teuthid_link_libraries ${teuthid_link_libraries})

include(CheckIncludeFileCXX)
option(BUILD_WITH_OPENCL "Build with OpenCL" ON)
option(USE_BOOST_COMPUTE "Using Boost.Compute for OpenCL" OFF)
option(BUILD_TESTS "Build the testing tree" ON)

include(TeuthidCheckBoost)

if (BUILD_WITH_OPENCL)
  include(TeuthidCheckOpenCL)
  if (NOT OpenCL_FOUND)
    set(BUILD_WITH_OPENCL OFF)
  endif()
endif()

set(BUILD_SHARED_LIBS ON)
option(BUILD_STATIC_LIBS "Build the static library" ON)

add_subdirectory(include)
add_subdirectory(src)

if (BUILD_TESTS AND Boost_UNIT_TEST_FRAMEWORK_FOUND)
  enable_testing()
  add_subdirectory(test)
else()
  set(BUILD_TESTS OFF)
endif()

option(BUILD_DOCUMENTATION "Build the documentation" ON)
if (BUILD_DOCUMENTATION)
  find_package(Doxygen QUIET)
  if (NOT DOXYGEN_FOUND)
    message(STATUS "Cannot find Doxygen! Building documentation is disabled")
    set(BUILD_DOCUMENTATION OFF)
  else()
    message(STATUS "Doxygen version: ${DOXYGEN_VERSION}")
    add_subdirectory(doc)
  endif()
endif()

#include(CheckCXXCompilerFlag)
#include(GNUInstallDirs)

message(STATUS "Teuthid version: ${teuthid_VERSION}\
  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build shared libraries: ${BUILD_SHARED_LIBS}\
  (Soversion: ${teuthid_library_version})" )
message(STATUS "Build static libraries: ${BUILD_STATIC_LIBS}")
message(STATUS "Build with OpenCL support: ${BUILD_WITH_OPENCL}")

foreach(libname_ ${teuthid_link_libraries})
  get_filename_component(libname_ ${libname_} NAME)
  list(APPEND libname_list_ ${libname_})  
endforeach()
string(REGEX REPLACE ";" " " libname_list_ "${libname_list_}")
message(STATUS "Linked libraries: ${libname_list_}")

message(STATUS "Build the testing tree: ${BUILD_TESTS}")
message(STATUS "Build the documentation: ${BUILD_DOCUMENTATION}")
message(STATUS "Install directory: ${CMAKE_INSTALL_PREFIX}")

