
include(CTest)

set(teuthid_tests
  class_library
)

foreach(test_ ${teuthid_tests})
  set(filename_ "${test_}.cpp")
  add_executable(${test_} "${filename_}")
  target_include_directories(${test_} PRIVATE "${teuthid_INCLUDE_DIR}")
  target_link_libraries(${test_} teuthid ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  add_test(${test_} ${test_})
endforeach()
