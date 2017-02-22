set(boost_required_components_ random)
list(APPEND boost_required_components_ 
  math_c99 math_c99f math_c99l math_tr1 math_tr1f math_tr1l)
if (BUILD_WITH_OPENCL AND USE_BOOST_COMPUTE)
  list(APPEND boost_required_components_ filesystem)
endif()

if (BUILD_TESTS)
  set(boost_optional_components_ unit_test_framework)
endif(BUILD_TESTS)

#set(Boost_REQUIRED_VERSION 1.61.0)
find_package(Boost 1.61.0 QUIET REQUIRED 
  COMPONENTS ${boost_required_components_}
  OPTIONAL_COMPONENTS ${boost_optional_components_}) 

if (NOT Boost_FOUND)
  message(FATAL_ERROR 
    "Cannot find Boost libraries: ${boost_required_components_}")
else()
  message(STATUS "Boost version:\
 ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
endif()

list(APPEND teuthid_link_libraries 
  ${Boost_SYSTEM_LIBRARIES} 
  ${Boost_MATH_C99_LIBRARIES} ${Boost_MATH_C99F_LIBRARIES}
  ${Boost_MATH_C99L_LIBRARIES}
  ${Boost_MATH_TR1_LIBRARIES} ${Boost_MATH_TR1F_LIBRARIES}
  ${Boost_MATH_TR1L_LIBRARIES}
  ${Boost_RANDOM_LIBRARIES})
if (BUILD_WITH_OPENCL AND USE_BOOST_COMPUTE)
  list(APPEND teuthid_link_libraries ${Boost_FILESYSTEM_LIBRARIES})
endif()

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND AND BUILD_TESTS)
  message(STATUS "Cannot find Boost.Test! Unit tests are disabled.")
endif()
