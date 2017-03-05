
configure_file(${teuthid_DOC_DIR}/doxyfile.in ${teuthid_DOC_DIR}/doxyfile @ONLY)
set(doxyfile ${teuthid_DOC_DIR}/doxyfile)

add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

