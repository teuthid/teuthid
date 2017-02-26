find_package(ArrayFire)

if(ArrayFire_FOUND)
  check_pkg_version(result_ ArrayFire_VERSION_MAJOR ArrayFire_VERSION_MINOR
    ArrayFire_VERSION_PATCH "check_af_version.cpp" ${ArrayFire_INCLUDE_DIRS}
    ${ArrayFire_CPU_LIBRARIES})
  if(result_)
    set(ArrayFire_VERSION 
    "${ArrayFire_VERSION_MAJOR}.${ArrayFire_VERSION_MINOR}.\
${ArrayFire_VERSION_PATCH}")
    msg_status("ArrayFire version: " 
      "${ArrayFire_VERSION} (required ${ArrayFire_REQUIRED_VERSION})")
  else()
    msg_error("Cannot determine version of ArrayFire!\
 Please check installation of ArrayFire on this system.")
  endif()

  check_pkg_required_version(result_ "${ArrayFire_REQUIRED_VERSION}" 
    ${ArrayFire_VERSION_MAJOR} ${ArrayFire_VERSION_MINOR} 
    ${ArrayFire_VERSION_PATCH})
  if (NOT result_)
    msg_error("Detected version of ArrayFire is too old.\
  Required version is ${ArrayFire_REQUIRED_VERSION} (or newer).")
  endif()
endif(ArrayFire_FOUND)

set(af_libs_)
set(af_backends_ " ")

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
  list_prepend(teuthid_INCLUDE_PATH ${ArrayFire_INCLUDE_DIRS})
  list(REMOVE_DUPLICATES teuthid_INCLUDE_PATH)

else()
  msg_warning(
    "Cannot find ArrayFire libraries! Build with ArrayFire is disabled.")
endif(ArrayFire_FOUND)

unset(af_libs_)
unset(af_backends_)
