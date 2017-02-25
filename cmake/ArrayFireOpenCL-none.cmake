#----------------------------------------------------------------
# Generated CMake target import file for configuration "None".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "afopencl" for configuration "None"
set_property(TARGET afopencl APPEND PROPERTY IMPORTED_CONFIGURATIONS NONE)
set_target_properties(afopencl PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NONE "clBLAS;clFFT"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NONE ""
  IMPORTED_LOCATION_NONE "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libafopencl.so.3.3.2"
  IMPORTED_SONAME_NONE "libafopencl.so.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS afopencl )
list(APPEND _IMPORT_CHECK_FILES_FOR_afopencl "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libafopencl.so.3.3.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
