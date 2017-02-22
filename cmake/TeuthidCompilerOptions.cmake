include(CheckCXXCompilerFlag)

set(list_avx_options_ avx avx2 __wrong__)
set(list_sse_options_ sse sse2 sse3 ssse3 sse4a sse4.1 sse4.2 __wrong__)

# GCC depresses SSEx instructions when -mavx is used. Instead, it generates new # AVX instructions or AVX equivalence for all SSEx instructions when needed. 
# To generate SSE/SSE2 instructions automatically from floating-point code
# (as opposed to 387 instructions), see -mfpmath=sse.
# These options are defined for the i386 and x86-64 family of computers.

function(teuthid_add_compiler_option option_ result_)
  check_cxx_compiler_flag("-m${option_}" moption_${option_})
  if (moption_${option_})
    set(result_ "-m${option_}" PARENT_SCOPE)
  else()
    set(result_ "" PARENT_SCOPE)
  endif()
endfunction(teuthid_add_compiler_option)

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
set(teuthid_tests_compiler_options ${teuthid_compiler_options})

set(found_AVX OFF)
set(found_SSE OFF)

if (ENABLE_AVX_SSE)
  message(STATUS "Check for working compiler options (AVX/SSE)")
  #set(teuthid_message_switch OFF)
  foreach(option_ ${list_avx_options_})
    teuthid_add_compiler_option("${option_}" result_)
    if (result_)
      set(teuthid_compiler_options "${teuthid_compiler_options} ${result_}")
      set(found_AVX_ ON)
    endif() 
    unset(result_)
  endforeach()
  if (found_AVX_)
    set(teuthid_compiler_options "${teuthid_compiler_options} -mfpmath=sse")
  endif()
  #set(teuthid_message_switch ON)
  message(STATUS "Check for working compiler options (AVX/SSE) - done") 
endif(ENABLE_AVX_SSE)

unset(found_AVX)
unset(found_SSE)
set(CMAKE_VERBOSE_MAKEFILE ON) # to comment out
