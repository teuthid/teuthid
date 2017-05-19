
set(Boost_USE_MULTITHREADED ON)
find_package(Boost ${Boost_REQUIRED_VERSION} QUIET REQUIRED 
  COMPONENTS ${Boost_REQUIRED_COMPONENTS}) 

if (NOT Boost_FOUND)
  string(REPLACE ";" ", " components_ "${Boost_REQUIRED_COMPONENTS}")
  msg_warning("Cannot find Boost components: ${components_}!")
  msg_warning("Unit tests are disabled.")
  set(BUILD_TESTS OFF)
else()
  msg_status("Boost version: " 
    "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}\
 (required ${Boost_REQUIRED_VERSION})")
  list(APPEND teuthid_tests_link_libraries ${Boost_LIBRARIES})
  list_prepend(teuthid_tests_INCLUDE_PATH ${Boost_INCLUDE_DIRS})
  list(REMOVE_DUPLICATES teuthid_tests_INCLUDE_PATH)
endif()
