#----------------------------------------------------------------
# Generated CMake target import file for configuration "None".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "afcpu" for configuration "None"
set_property(TARGET afcpu APPEND PROPERTY IMPORTED_CONFIGURATIONS NONE)
set_target_properties(afcpu PROPERTIES
  IMPORTED_LOCATION_NONE "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libafcpu.so.3.3.2"
  IMPORTED_SONAME_NONE "libafcpu.so.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS afcpu )
list(APPEND _IMPORT_CHECK_FILES_FOR_afcpu "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libafcpu.so.3.3.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
