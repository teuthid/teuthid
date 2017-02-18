find_package(OpenCL QUIET)

if (NOT OpenCL_FOUND)
  message(STATUS "Cannot find OpenCL! Build with OpenCL is disabled.")
else()
  message(STATUS "OpenCL version: ${OpenCL_VERSION_STRING}")
endif()

if (OpenCL_FOUND)
  message(STATUS "Checking OpenCL platforms and devices on this system ...")
  try_run(run_result_ compile_result_ ${teuthid_CMAKE_DIR}
    "${teuthid_CMAKE_DIR}/opencl_test.cpp" LINK_LIBRARIES ${OpenCL_LIBRARIES}
    COMPILE_OUTPUT_VARIABLE compile_output_ RUN_OUTPUT_VARIABLE run_output_)
  if (run_result_ EQUAL 0)
    message(STATUS "OpenCL platforms:")
    message("${run_output_}")
  else()
    message(STATUS 
      "Cannot find OpenCL platforms! Build with OpenCL is disabled.")
    set(OpenCL_FOUND OFF)
  endif()
endif()

set(TEUTHID_USE_OPENCL ${OpenCL_FOUND})
set(BOOST_COMPUTE_USE_OFFLINE_CACHE ${OpenCL_FOUND})
if (OpenCL_FOUND)
  list(APPEND teuthid_link_libraries ${OpenCL_LIBRARIES})
endif()

# message("${teuthid_link_libraries}")