#----------------------------------------------------------------
# Generated CMake target import file for configuration "None".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "af" for configuration "None"
set_property(TARGET af APPEND PROPERTY IMPORTED_CONFIGURATIONS NONE)
set_target_properties(af PROPERTIES
  IMPORTED_LOCATION_NONE "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libaf.so.3.3.2"
  IMPORTED_SONAME_NONE "libaf.so.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS af )
list(APPEND _IMPORT_CHECK_FILES_FOR_af "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libaf.so.3.3.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
