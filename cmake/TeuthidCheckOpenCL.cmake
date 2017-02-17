
find_package(OpenCL QUIET)

if (NOT OpenCL_FOUND)
  message(STATUS "Cannot find OpenCL! Build with OpenCL is disabled.")
else()
  message(STATUS "OpenCL version: ${OpenCL_VERSION_STRING}")
  message(STATUS "OpenCL libraries:")
  message(STATUS "    ${OpenCL_LIBRARIES}")
endif()

if (OpenCL_FOUND)
  try_run(run_result_ compile_result_ ${teuthid_CMAKE_DIR}
    "${teuthid_CMAKE_DIR}/opencl_test.cpp" 
    LINK_LIBRARIES ${OpenCL_LIBRARIES} ${Boost_LIBRARIES} 
    COMPILE_OUTPUT_VARIABLE compile_output_
    RUN_OUTPUT_VARIABLE run_output_ )
  
  message(${compile_output_})
  message(${run_output_})
endif()



set(TEUTHID_WITH_OPENCL ${OpenCL_FOUND})
set(BOOST_COMPUTE_USE_OFFLINE_CACHE ${OpenCL_FOUND})

# -DBOOST_COMPUTE_USE_OFFLINE_CACHE will enable the offline kernel cache 
# (this requires linking with boost_filesystem). Then the kernels you use will 
# be stored in your file system and only be compiled the very first time you
# run your application (NVIDIA on Linux already does this by default).

