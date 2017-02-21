include(CTest)

set(teuthid_tests
  class_library
)

foreach(test_ ${teuthid_tests})
  set(filename_ "${test_}.cpp")
  add_executable(${test_} "${filename_}")
  target_include_directories(${test_} PRIVATE ${teuthid_INCLUDE_PATH})
  target_link_libraries(${test_} teuthid ${teuthid_link_libraries}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
  # target_compile_options(${test_} PRIVATE 

  add_test(${test_} ${test_})
endforeach()
