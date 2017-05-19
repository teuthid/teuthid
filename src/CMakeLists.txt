set(teuthid_library_sources
  system.cpp
)

if (BUILD_WITH_OPENCL)
  set(teuthid_clb_library_sources
    clb/cl2.hpp clb/error.cpp clb/platform.cpp clb/device.cpp
  )
  list(APPEND teuthid_library_sources ${teuthid_clb_library_sources})
endif(BUILD_WITH_OPENCL)

set(compiler_options_ "${AVX_SSE_compiler_options}")
separate_arguments(compiler_options_)

if (BUILD_SHARED_LIBS)
  add_library(teuthid SHARED ${teuthid_library_sources})
  target_include_directories(teuthid PUBLIC ${teuthid_INCLUDE_PATH})
  target_link_libraries(teuthid ${teuthid_link_libraries})
  target_compile_options(teuthid PUBLIC ${compiler_options_})
  set_target_properties(teuthid PROPERTIES 
    SOVERSION ${teuthid_library_soversion} VERSION ${teuthid_library_version})
  install(TARGETS teuthid DESTINATION lib)
endif(BUILD_SHARED_LIBS)

if (BUILD_STATIC_LIBS)
  add_library(teuthid-static STATIC ${teuthid_library_sources})
  target_include_directories(teuthid-static PUBLIC ${teuthid_INCLUDE_PATH})
  target_link_libraries(teuthid-static ${teuthid_link_libraries})
  target_compile_options(teuthid-static PUBLIC ${compiler_options_})
  set_target_properties(teuthid-static PROPERTIES OUTPUT_NAME "teuthid")
  set_target_properties(teuthid-static PROPERTIES PREFIX "lib")
  install(TARGETS teuthid-static DESTINATION lib)
endif(BUILD_STATIC_LIBS)

unset(compiler_options_)