include(CTest)

set(teuthid_tests
  class_floatmp class_system
)

if (BUILD_WITH_OPENCL)
  set(teuthid_clb_tests
    class_clb_error class_clb_device class_clb_platform
  )
  list(APPEND teuthid_tests ${teuthid_clb_tests})
endif()

set(compiler_options_ "${AVX_SSE_compiler_options}")
separate_arguments(compiler_options_)

foreach(test_ ${teuthid_tests})
  set(filename_ "${test_}.cpp")
  add_executable(${test_} "${filename_}")
  target_include_directories(${test_} PRIVATE ${teuthid_tests_INCLUDE_PATH})
  target_link_libraries(${test_} teuthid ${teuthid_tests_link_libraries})
  target_compile_options(${test_} PRIVATE ${compiler_options_})
  add_test(${test_} ${test_})
endforeach()
