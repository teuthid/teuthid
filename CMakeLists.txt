cmake_minimum_required(VERSION 3.5.0)

set(teuthid_message_switch ON)
include(cmake/TeuthidMessage.cmake)

#==============================================================================

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
  msg_error("Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH 
    "default install path" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
SET(CMAKE_COLOR_MAKEFILE ON)

option(ENABLE_AVX_SSE "Enable AVX or SSE compiler options" ON)
option(ENABLE_AVX_SSE_FOR_TESTS 
  "Enable AVX or SSE compiler options for unit tests only" OFF)

set(teuthid_compiler_options)
set(teuthid_tests_compiler_options)
include(TeuthidCompilerOptions)

set(teuthid_INCLUDE_PATH ${teuthid_SOURCE_DIR}/include)
set(teuthid_INCLUDE_DIR ${teuthid_SOURCE_DIR}/include/teuthid)
set(teuthid_link_libraries ${teuthid_link_libraries})

set(BUILD_SHARED_LIBS ON)
option(BUILD_STATIC_LIBS "Build the static library" ON)
option(BUILD_WITH_OPENCL "Build with OpenCL" OFF)
option(USE_BOOST_COMPUTE "Use Boost.Compute for OpenCL" ON)
option(BUILD_TESTS "Build the testing tree" ON)
option(BUILD_DOCS "Build the documentation" ON)
option(BUILD_EXAMPLES "Build examples" ON)

include(TeuthidCheckBoost)

if (BUILD_WITH_OPENCL)
  include(TeuthidCheckOpenCL)
  if (NOT OpenCL_FOUND)
    set(BUILD_WITH_OPENCL OFF)
  endif()
endif(BUILD_WITH_OPENCL)

add_subdirectory(include)
add_subdirectory(src)

if (BUILD_TESTS AND Boost_UNIT_TEST_FRAMEWORK_FOUND)
  enable_testing()
  add_subdirectory(test)
else()
  set(BUILD_TESTS OFF)
endif(BUILD_TESTS AND Boost_UNIT_TEST_FRAMEWORK_FOUND)

if (BUILD_DOCS)
  find_package(Doxygen QUIET)
  if (NOT DOXYGEN_FOUND)
    msg_warning("Cannot find Doxygen! Building documentation is disabled.")
    set(BUILD_DOCS OFF)
  else()
    msg_status("Doxygen version: " "${DOXYGEN_VERSION}")
    add_subdirectory(doc)
  endif()
endif(BUILD_DOCS)

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif(BUILD_EXAMPLES)

configure_file(${teuthid_SOURCE_DIR}/README.in 
               ${teuthid_SOURCE_DIR}/README)
# TODO: install README AUTHORS etc.

message("--")
msg_status("Teuthid version: "  "${teuthid_VERSION}")
msg_status("Build type: " 
  "${CMAKE_BUILD_TYPE} (${CMAKE_C_COMPILER_ID} ${CMAKE_SYSTEM}\
 ${CMAKE_SYSTEM_PROCESSOR})")
msg_status("Compiler options: " 
  "${teuthid_compiler_options} ${AVX_SSE_compiler_options}")
msg_status("Compiler options for unit tests: " 
  "${teuthid_tests_compiler_options} ${AVX_SSE_compiler_options_for_tests}")
msg_status("Build shared libraries: "
  "${BUILD_SHARED_LIBS} (Soversion: ${teuthid_library_version})")
msg_status("Build static libraries: " "${BUILD_STATIC_LIBS}")
msg_status("Build with OpenCL support: " "${BUILD_WITH_OPENCL}")

foreach(libname_ ${teuthid_link_libraries})
  get_filename_component(libname_ ${libname_} NAME)
  list(APPEND libname_list_ ${libname_})  
endforeach()
string(REPLACE ";" ", " libname_list_ "${libname_list_}")
msg_status("Linked libraries: " "${libname_list_}")

msg_status("Build the unit tests: " "${BUILD_TESTS}")
msg_status("Build the documentation: " "${BUILD_DOCS}")
msg_status("Install directory: " "${CMAKE_INSTALL_PREFIX}")

message("--")