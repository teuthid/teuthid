set(boost_required_components_ random)
list(APPEND boost_required_components_ 
  math_c99 math_c99f math_c99l math_tr1 math_tr1f math_tr1l)
if (BUILD_WITH_OPENCL AND USE_BOOST_COMPUTE)
  list(APPEND boost_required_components_ filesystem)
endif()

if (BUILD_TESTS)
  set(boost_optional_components_ unit_test_framework)
endif(BUILD_TESTS)

set(Boost_REQUIRED_VERSION 1.61.0)
find_package(Boost ${Boost_REQUIRED_VERSION} QUIET REQUIRED 
  COMPONENTS ${boost_required_components_}
  OPTIONAL_COMPONENTS ${boost_optional_components_}) 

if (NOT Boost_FOUND)
  string(REPLACE ";" ", " components_ "${boost_required_components_}")
  msg_error("Cannot find required Boost components: ${components_}!")
else()
  msg_status("Boost version: " 
    "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
endif()

list(APPEND teuthid_link_libraries ${Boost_LIBRARIES})

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND AND BUILD_TESTS)
  msg_warning("Cannot find Boost.Test! Unit tests are disabled.")
endif()

unset(boost_required_components_)
unset(boost_optional_components_)
