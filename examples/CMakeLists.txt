set(teuthid_examples
  hello
)

foreach(example_ ${teuthid_examples})
  set(filename_ "${example_}.cpp")
  add_executable(${example_} "${filename_}")
  target_include_directories(${example_} PRIVATE ${teuthid_INCLUDE_PATH})
  target_link_libraries(${example_} teuthid ${teuthid_link_libraries})
endforeach()
