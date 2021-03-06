find_package(OpenCL)

if (NOT OpenCL_FOUND)
  msg_warning(
    "Cannot find OpenCL libraries/headers! Build with OpenCL is disabled.")
else()
  msg_status("OpenCL version (header): " "${OpenCL_VERSION_STRING}")
endif()

if (OpenCL_FOUND AND ${CHECK_OPENCL_DEVICES})
  message(STATUS "Check for OpenCL platforms/devices")
  try_run(run_result_ compile_result_ ${teuthid_CMAKE_DIR}
    "${teuthid_CMAKE_DIR}/checks/check_cl_devices.c"
    LINK_LIBRARIES ${OpenCL_LIBRARIES}
    COMPILE_OUTPUT_VARIABLE compile_output_ RUN_OUTPUT_VARIABLE run_output_)
  file(WRITE "info.log" ${run_output_})
  if (run_result_ EQUAL 0)
    file(STRINGS "info.log" cl_info_)
    list(REVERSE cl_info_)
    list(GET cl_info_ 0 driver_version_)
    message(STATUS "Check for OpenCL platforms/devices -- works")
    msg_status("OpenCL version (driver): " "${driver_version_}")
  else()
    message(STATUS "Check for OpenCL platforms/devices -- ${Red}failed!\
 Build with OpenCL is disabled.${ColorReset}")
    set(OpenCL_FOUND OFF)
  endif()
endif()  

set(TEUTHID_WITH_OPENCL ${OpenCL_FOUND})
if (OpenCL_FOUND)
  list(APPEND teuthid_link_libraries ${OpenCL_LIBRARIES})
  list_prepend(teuthid_INCLUDE_PATH ${OpenCL_INCLUDE_DIRS})
  list(REMOVE_DUPLICATES teuthid_INCLUDE_PATH)
endif()

unset(msg_)