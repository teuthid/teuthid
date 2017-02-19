set(boost_required_components_ random)
set(boost_optional_components_ unit_test_framework)
find_package(Boost 1.61.0 QUIET REQUIRED 
  COMPONENTS ${boost_required_components_}
  OPTIONAL_COMPONENTS ${boost_optional_components_}) 

if (NOT Boost_FOUND)
  message(FATAL_ERROR "Cannot find Boost libraries: Random")
else()
  message(STATUS "Boost version:\
    ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
endif()
list(APPEND teuthid_link_libraries 
  ${Boost_SYSTEM_LIBRARIES} 
  ${Boost_RANDOM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES})

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
  message(STATUS "Cannot find Boost.Test! Unit tests are disabled.")
endif()
