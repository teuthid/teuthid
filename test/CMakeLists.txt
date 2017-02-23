include(CTest)

set(teuthid_tests
  class_library
)

set(compiler_options_ "${AVX_SSE_compiler_options_for_tests}")
separate_arguments(compiler_options_)

foreach(test_ ${teuthid_tests})
  set(filename_ "${test_}.cpp")
  add_executable(${test_} "${filename_}")
  target_include_directories(${test_} PRIVATE ${teuthid_INCLUDE_PATH})
  target_link_libraries(${test_} teuthid ${teuthid_link_libraries})
  target_compile_options(${test_} PRIVATE ${compiler_options_})
  add_test(${test_} ${test_})
endforeach()

unset(compiler_options_)