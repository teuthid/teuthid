include(CheckCXXCompilerFlag)

set(extra_options_ "")
set(list_ext_ avx avx2 sse2 sse3 ssse3 sse4a sse4.1 sse4.2 sse5)

function(teuthid_add_cxx_compiler_option option_)
  string(REGEX REPLACE "-" "" soption_ ${option_})
  check_cxx_compiler_flag(${option_} compiler_option_${soption_})
  if (compiler_option_${soption_})
    set(extra_options_ "${extra_options_} ${option_}" PARENT_SCOPE)
  endif()
endfunction(teuthid_add_cxx_compiler_option)

set(str_list_ext_ "")
foreach(ext_ ${list_ext_})
  set(str_list_ext_ "${str_list_ext_} ${ext_}")
endforeach()
msg_status("Testing compiler options:" "${str_list_ext_}")

set(teuthid_message_switch OFF)
foreach(ext_ ${list_ext_})
  teuthid_add_cxx_compiler_option("-m${ext_}")
endforeach()
set(teuthid_message_switch ON)
set(teuthid_tests_compiler_avx_sse_options "${extra_options_}")

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set(teuthid_compiler_options ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(teuthid_compiler_options ${CMAKE_CXX_FLAGS_RELEASE})
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(teuthid_compiler_options ${CMAKE_CXX_FLAGS_DEBUG})
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
  set(teuthid_compiler_options ${CMAKE_CXX_FLAGS_MINSIZEREL})
else()
  set(teuthid_compiler_options ${CMAKE_CXX_FLAGS}) # FIXME
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)