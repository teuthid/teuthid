include(CheckCXXCompilerFlag)

set(list_AVX_options_ avx avx2)
set(list_SSE_options_ sse sse2 sse3 ssse3 sse4a sse4.1 sse4.2)

# GCC depresses SSEx instructions when -mavx is used. Instead, it generates new # AVX instructions or AVX equivalence for all SSEx instructions when needed. 
# To generate SSE/SSE2 instructions automatically from floating-point code
# (as opposed to 387 instructions), see -mfpmath=sse.
# These options are defined for the i386 and x86-64 family of computers.

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

macro(teuthid_add_compiler_option option_ result_)
  set(teuthid_message_switch OFF)
  check_cxx_compiler_flag("-m${option_}" moption_${option_})
  set(teuthid_message_switch ON)
  if (moption_${option_})
    set(result_ "-m${option_}")
  endif()
endmacro(teuthid_add_compiler_option)

macro(teuthid_add_compiler_options options_)
  set(checked_ext_, "")
  foreach(ext_ ${list_${options_}_options_})
    teuthid_add_compiler_option("${ext_}" result_)
    if (result_)
      set(checked_ext_ "${checked_ext_} ${result_}")
      set(found_extensions_ ${options_})
    endif()
    unset(result_)
  endforeach()
  if (checked_ext_)
    set(checked_ext_ "${checked_ext_} -mfpmath=sse")
  endif()
  if (ENABLE_AVX_SSE AND checked_ext_)
    set(AVX_SSE_compiler_options "${AVX_SSE_compiler_options}${checked_ext_}")
  endif()
  if (ENABLE_AVX_SSE_FOR_TESTS AND checked_ext_)
  set(AVX_SSE_compiler_options_for_tests 
    "${AVX_SSE_compiler_options_for_tests}${checked_ext_}")
  endif()
  unset(checked_ext_)
endmacro()

set(found_extensions_ NONE)
set(AVX_SSE_compiler_options " ")
set(AVX_SSE_compiler_options_for_tests " ")

if (ENABLE_AVX_SSE OR ENABLE_AVX_SSE_FOR_TESTS)
  message(STATUS "Check for working compiler options (AVX/SSE)")
  teuthid_add_compiler_options(AVX)
  if (${found_extensions_} MATCHES NONE)
    teuthid_add_compiler_options(SSE)
  endif()
  if (NOT (${found_extensions_} MATCHES NONE))
    msg_status("Check for working compiler options (AVX/SSE) - " 
      "${found_extensions_}")
  else()
    msg_status("Check for working compiler options (AVX/SSE) - " 
      "${Yellow}not found${ColorReset}")
  endif()
endif()

unset(found_extensions_)
set(CMAKE_VERBOSE_MAKEFILE ON) # to comment out
