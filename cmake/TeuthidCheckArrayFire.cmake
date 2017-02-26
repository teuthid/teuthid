find_package(ArrayFire)
set(ArrayFire_REQUIRED_VERSION "3.3.2")

macro(check_af_version_)
  try_run(run_result_ compile_result_ ${teuthid_CMAKE_DIR}
    "${teuthid_CMAKE_DIR}/checks/check_af_version.cpp"
    COMPILE_OUTPUT_VARIABLE compile_output_ RUN_OUTPUT_VARIABLE run_output_)
  if (run_result_ EQUAL 0)
    file(WRITE "info.log" ${run_output_})
    file(STRINGS "info.log" af_version_)
    list(GET af_version_ 0 ArrayFire_VERSION_MAJOR)
    list(GET af_version_ 1 ArrayFire_VERSION_MINOR)
    list(GET af_version_ 2 ArrayFire_VERSION_PATCH)
    set(ArrayFire_VERSION_STRING "${ArrayFire_VERSION_MAJOR}.\
${ArrayFire_VERSION_MINOR}.${ArrayFire_VERSION_PATCH}")
    math(EXPR ArrayFire_VERSION 
      "100 * ${ArrayFire_VERSION_MAJOR} + 10 * ${ArrayFire_VERSION_MINOR} + \
      ${ArrayFire_VERSION_PATCH}")
  else() # broken af/version.h
    msg_error("Cannot determine version of ArrayFire!\
 Please check installation of ArrayFire on this system.")
  endif()
endmacro(check_af_version_)

function(check_required_version_ required_version_)
  string(REPLACE "." ";" version_list_ ${required_version_})
  list(GET version_list_ 0 major_required_)
  list(GET version_list_ 1 minor_required_)
  list(GET version_list_ 2 patch_required_)
  math(EXPR required_ 
    "100 * ${major_required_} + 10 * ${minor_required_} + ${patch_required_}")
  if (${ArrayFire_VERSION} LESS required_)
    msg_error("Detected version of ArrayFire is too old.\
  Required version is ${required_version_} (or newer).")
  endif()
endfunction(check_required_version_)

set(af_libs_)
set(af_backends_ " ")

if(ArrayFire_FOUND)
  check_af_version_()
  msg_status("ArrayFire version: " 
    "${ArrayFire_VERSION_STRING} (required ${ArrayFire_REQUIRED_VERSION})")
  check_required_version_(${ArrayFire_REQUIRED_VERSION})
endif(ArrayFire_FOUND)

if (ArrayFire_FOUND)
  
  if (ArrayFire_CUDA_FOUND)
    list(APPEND af_libs_ ${ArrayFire_CUDA_LIBRARIES})
    set(af_backends_ "CUDA")
    set(ARRAYFIRE_CUDA_BACKEND ON)
  endif(ArrayFire_CUDA_FOUND)

  if (ArrayFire_OpenCL_FOUND AND BUILD_WITH_OPENCL)
    if(OpenCL_FOUND)
      set(TEUTHID_USE_OPENCL ${OpenCL_FOUND})
      set(ARRAYFIRE_OPENCL_BACKEND ON)
      list(APPEND af_libs_ ${ArrayFire_OpenCL_LIBRARIES})
      set(af_backends_ "${af_backends_} OpenCL")
    endif(OpenCL_FOUND)
  endif(ArrayFire_OpenCL_FOUND AND BUILD_WITH_OPENCL)

  if(ArrayFire_CPU_FOUND)
    list(APPEND af_libs_ ${ArrayFire_CPU_LIBRARIES})
    set(af_backends_ "${af_backends_} CPU")
    set(ARRAYFIRE_CPU_BACKEND ON)
  endif(ArrayFire_CPU_FOUND)
  
  string(STRIP "${af_backends_}" af_backends_)
  string(REPLACE " " ", " af_backends_ "${af_backends_}")
  msg_status("ArrayFire backend(s): " "${af_backends_}")
  set(TEUTHID_USE_ARRAYFIRE ON)
  list(APPEND teuthid_link_libraries ${af_libs_})
  if (ArrayFire_Unified_FOUND)
    list(APPEND teuthid_link_libraries ${ArrayFire_Unified_LIBRARIES})
  endif(ArrayFire_Unified_FOUND)
  list(APPEND teuthid_INCLUDE_PATH ${ArrayFire_INCLUDE_DIRS})
  list(REMOVE_DUPLICATES teuthid_INCLUDE_PATH)

else()
  msg_warning(
    "Cannot find ArrayFire libraries! Build with ArrayFire is disabled.")
endif(ArrayFire_FOUND)

unset(af_libs_)
unset(af_backends_)
