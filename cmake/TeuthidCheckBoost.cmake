
find_package(Boost 1.61.0 QUIET REQUIRED COMPONENTS filesystem random
  OPTIONAL_COMPONENTS unit_test_framework) 

if (NOT Boost_FOUND)
  message(FATAL_ERROR "Cannot find Boost libraries: Random")
else()
  message(STATUS "Boost version:\
    ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
endif()

if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
  message(STATUS "Cannot find Boost.Test! Unit tests are disabled.")
endif()

message(STATUS "Boost libraries:")
foreach(lib_ ${Boost_LIBRARIES})
  message("   ${lib_}")
endforeach()
