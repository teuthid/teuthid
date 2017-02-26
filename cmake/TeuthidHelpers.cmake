
# Example: list_prepend(teuthid_INCLUDE_PATH ${Boost_INCLUDE_DIRS})
function(list_prepend list_ elem_)
  list(LENGTH list_ size_)
  if(${size_} GREATER 0)
    set(tmp_ ${${list_}})
    list(REVERSE tmp_)
    list(APPEND tmp_ ${elem_})
    list(REVERSE tmp_)
    set(${list_} ${tmp_} PARENT_SCOPE)
  else()
    set(${list_} ${elem_} PARENT_SCOPE)
  endif()
endfunction()


# Example: 
# check_pkg_version(result_ 
#                   GMP_VERSION_MAJOR 
#                   GMP_VERSION_MINOR 
#                   GMP_VERSION_PATCH
#                   "check_gmp_version.cpp" 
#                   ${GMP_INCLUDE_DIRS}
#                   ${GMP_LIBRARIES} )
function(check_pkg_version 
           result_ major_ minor_ patch_ # output vars
           srcfile_ headers_ libs_) # input vars
  try_run(run_result_ compile_result_ ${teuthid_CMAKE_DIR}
    "${teuthid_CMAKE_DIR}/checks/${srcfile_}"
    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${headers_}"
    LINK_LIBRARIES ${libs_}
    COMPILE_OUTPUT_VARIABLE compile_output_ RUN_OUTPUT_VARIABLE run_output_)
  file(WRITE "errors.log" ${compile_output_})
  file(WRITE "info.log" ${run_output_})
  if (run_result_ EQUAL 0)
    file(STRINGS "info.log" version_)
    list(GET version_ 0 tmp_)
    set(${major_} ${tmp_} PARENT_SCOPE) 
    list(GET version_ 1 tmp_)
    set(${minor_} ${tmp_} PARENT_SCOPE) 
    list(GET version_ 2 tmp_)
    set(${patch_} ${tmp_} PARENT_SCOPE) 
    set(${result_} ON PARENT_SCOPE)
  else()
    set(${result_} OFF PARENT_SCOPE)
  endif()
endfunction()


# Example:
# check_pkg_required_version(result_ 
#                            "${GMP_REQUIRED_VERSION}"
#                             ${GMP_VERSION_MAJOR}
#                             ${GMP_VERSION_MINOR}
#                             ${GMP_VERSION_PATCH})
function(check_pkg_required_version 
           result_ required_version_ major_ minor_ patch_)
  string(REPLACE "." ";" version_ ${required_version_})
  list(GET version_ 0 major_required_)
  list(GET version_ 1 minor_required_)
  list(GET version_ 2 patch_required_)
  math(EXPR required_ 
    "100 * ${major_required_} + 10 * ${minor_required_} + ${patch_required_}")
  math(EXPR actual_ "100 * ${major_} + 10 * ${minor_} + ${patch_}")
  if(actual_ LESS required_)
    set(${result_} OFF PARENT_SCOPE)
  else()
    set(${result_} ON PARENT_SCOPE)
  endif()
endfunction()
