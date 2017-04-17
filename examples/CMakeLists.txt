set(teuthid_examples
)

if (BUILD_WITH_OPENCL)
  set(teuthid_clb_examples
    cl_info
  )
  list(APPEND teuthid_examples ${teuthid_clb_examples})
endif(BUILD_WITH_OPENCL)

foreach(example_ ${teuthid_examples})
  set(filename_ "${example_}.cpp")
  add_executable(${example_} "${filename_}")
  target_include_directories(${example_} PRIVATE ${teuthid_INCLUDE_PATH})
  target_link_libraries(${example_} teuthid ${teuthid_link_libraries})
endforeach()
