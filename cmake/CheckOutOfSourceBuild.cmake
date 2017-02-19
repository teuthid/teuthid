string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource_)
get_filename_component(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PARENTDIR}" insourcesubdir_)
if (insource_ OR insourcesubdir_)
  message(FATAL_ERROR "Teuthid requires an out of source build!")
endif()

