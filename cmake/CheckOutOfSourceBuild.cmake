# Check if doing out of source build
macro(CHECK_OUT_OF_SOURCE_BUILD message_)
  string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource_)
  get_filename_component(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
  string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PARENTDIR}" insourcesubdir_)
  if (insource_ OR insourcesubdir_)
    message(FATAL_ERROR "${message_}")
  endif()
endmacro(CHECK_OUT_OF_SOURCE_BUILD)
