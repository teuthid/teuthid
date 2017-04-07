configure_file(${teuthid_INCLUDE_DIR}/config.hpp.in 
  ${teuthid_INCLUDE_DIR}/config.hpp)

install(DIRECTORY teuthid/ DESTINATION include/teuthid 
  FILES_MATCHING PATTERN "*.hpp")
