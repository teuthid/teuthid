find_package(OpenMP REQUIRED)

set(teuthid_compiler_options "${teuthid_compiler_options} ${OpenMP_CXX_FLAGS}")
#list(APPEND teuthid_link_libraries ${OpenMP_CXX_FLAGS})
