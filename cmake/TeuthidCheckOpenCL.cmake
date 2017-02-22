find_package(OpenCL QUIET)

if (NOT OpenCL_FOUND)
  msg_warning("Cannot find OpenCL! Build with OpenCL is disabled.")
else()
  msg_status("OpenCL version: " "${OpenCL_VERSION_STRING}")
endif()

if (OpenCL_FOUND)
  message(STATUS "Checking OpenCL platforms and devices on this system ...")
  try_run(run_result_ compile_result_ ${teuthid_CMAKE_DIR}
    "${teuthid_CMAKE_DIR}/opencl_test.cpp" LINK_LIBRARIES ${OpenCL_LIBRARIES}
    COMPILE_OUTPUT_VARIABLE compile_output_ RUN_OUTPUT_VARIABLE run_output_)
  if (run_result_ EQUAL 0)
    message(STATUS "Found OpenCL platforms:")
    msg_status("" "${run_output_}")
  else()
    msg_warning("Cannot find OpenCL platforms! Build with OpenCL is disabled.")
    set(OpenCL_FOUND OFF)
  endif()

  if (NOT USE_BOOST_COMPUTE)
    # check if CL/cl2.hpp (C++ bindings for OpenCL) is working
    set(open_cl_version_ "${OpenCL_VERSION_MAJOR}${OpenCL_VERSION_MINOR}0")
    set(msg_ 
      "Checking CL/cl2.hpp with OpenCL version ${OpenCL_VERSION_STRING} ...")
    message(STATUS "${msg_}")
    try_compile(compile_result_ ${teuthid_CMAKE_DIR}
      "${teuthid_CMAKE_DIR}/cl2_hpp_test.cpp" 
      COMPILE_DEFINITIONS "-DCL_HPP_TARGET_OPENCL_VERSION=${open_cl_version_}"
      LINK_LIBRARIES ${OpenCL_LIBRARIES} OUTPUT_VARIABLE compile_output_)
      if (compile_result_)
        msg_status("${msg_} " "OK")
      else()
        msg_status("${msg_} " "${Red}Failed${ColorReset}")
        msg_warning("Building with OpenCL is disabled.")
        set(OpenCL_FOUND OFF)  
      endif(compile_result_)
    
  else() # check if boost.compute is working
    set(msg_ 
      "Checking Boost.Compute with OpenCL version ${OpenCL_VERSION_STRING} ...")
    message(STATUS "${msg_}")
    try_compile(compile_result_ ${teuthid_CMAKE_DIR}
      "${teuthid_CMAKE_DIR}/boost_compute_test.cpp" 
      LINK_LIBRARIES ${OpenCL_LIBRARIES} OUTPUT_VARIABLE compile_output_)
    if (compile_result_)
      msg_status("${msg_} " "OK")
    else()
      msg_status("${msg_} " "${Red}Failed${ColorReset}")
      msg_warning("Building with OpenCL is disabled.")
      set(OpenCL_FOUND OFF)
    endif(compile_result_)
  endif()
endif(OpenCL_FOUND)

set(TEUTHID_USE_OPENCL ${OpenCL_FOUND})
if (OpenCL_FOUND)
  list(APPEND teuthid_link_libraries ${OpenCL_LIBRARIES})
  if (USE_BOOST_COMPUTE)
    set(TEUTHID_USE_BOOST_COMPUTE ON)
    set(BOOST_COMPUTE_USE_OFFLINE_CACHE ON)
  else()
    set(CL_HPP_TARGET_OPENCL_VERSION ON)
  endif(USE_BOOST_COMPUTE)
endif(OpenCL_FOUND)

unset(msg_)