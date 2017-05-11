if (BUILD_WITH_OPENCL)
  list(APPEND Boost_REQUIRED_COMPONENTS filesystem)
endif()

if (BUILD_TESTS)
  set(boost_optional_components_ unit_test_framework)
endif(BUILD_TESTS)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost ${Boost_REQUIRED_VERSION} REQUIRED 
  COMPONENTS ${Boost_REQUIRED_COMPONENTS}
  OPTIONAL_COMPONENTS ${boost_optional_components_}) 

if (NOT Boost_FOUND)
  string(REPLACE ";" ", " components_ "${Boost_REQUIRED_COMPONENTS}")
  msg_error("Cannot find required Boost components: ${components_}!")
else()
  msg_status("Boost version: " 
    "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}\
 (required ${Boost_REQUIRED_VERSION})")
endif()

list(APPEND teuthid_link_libraries ${Boost_LIBRARIES})
list_prepend(teuthid_INCLUDE_PATH ${Boost_INCLUDE_DIRS})
list(REMOVE_DUPLICATES teuthid_INCLUDE_PATH)

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND AND BUILD_TESTS)
  msg_warning("Cannot find Boost.Test! Unit tests are disabled.")
endif()
