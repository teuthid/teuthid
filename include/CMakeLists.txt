
add_subdirectory(teuthid)

install(DIRECTORY teuthid/ DESTINATION include/teuthid 
  FILES_MATCHING PATTERN "*.hpp")
    