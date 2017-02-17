
set(teuthid_library_sources
  library.cpp
)

if (BUILD_SHARED_LIBS)
  add_library(teuthid SHARED ${teuthid_library_sources})
  target_include_directories(teuthid PUBLIC "${teuthid_INCLUDE_DIR}")
  set_target_properties(teuthid PROPERTIES 
    SOVERSION ${teuthid_library_soversion} VERSION ${teuthid_library_version})
  install(TARGETS teuthid DESTINATION lib)
endif()

if (BUILD_STATIC_LIBS)
  add_library(teuthid-static STATIC ${teuthid_library_sources})
  target_include_directories(teuthid-static PUBLIC "${teuthid_INCLUDE_DIR}")
  set_target_properties(teuthid-static PROPERTIES OUTPUT_NAME "teuthid")
  set_target_properties(teuthid-static PROPERTIES PREFIX "lib")
  install(TARGETS teuthid-static DESTINATION lib)
endif()

# after the target_include_directories(...) above you don't need the other 
# include_directories() call. 

