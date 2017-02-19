set(boost_required_components_ random)
if (BUILD_WITH_OPENCL AND USE_BOOST_COMPUTE)
  list(APPEND boost_required_components_ filesystem)
endif()

if (BUILD_TESTS)
  set(boost_optional_components_ unit_test_framework)
endif()

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
  ${Boost_SYSTEM_LIBRARIES} ${Boost_RANDOM_LIBRARIES})
if (BUILD_WITH_OPENCL AND USE_BOOST_COMPUTE)
  list(APPEND teuthid_link_libraries ${Boost_FILESYSTEM_LIBRARIES})
endif()

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND AND BUILD_TESTS)
  message(STATUS "Cannot find Boost.Test! Unit tests are disabled.")
endif()
