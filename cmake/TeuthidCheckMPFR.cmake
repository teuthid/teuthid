
# MPFR_FOUND - TRUE if GMP is available.
# MPFR_VERSION - Version of GMP.
# MPFR_VERSION_MAJOR, MPFR_VERSION_MINOR, MPFR_VERSION_PATCH
# MPFR_INCLUDE_DIRS - Should be passed to target_include_directories() if
#                    the target is not used for linking.
# MPFR_LIBRARIES - Can be passed to target_link_libraries().

include(TeuthidCheckGMP)

set(MPFR_FOUND OFF)
find_path(MPFR_INCLUDE_DIRS NAMES mpfr.h)
find_library(MPFR_LIBRARIES NAMES mpfr libmpfr)
if ( (${MPFR_INCLUDE_DIRS} MATCHES "NOTFOUND") OR
    (${MPFR_LIBRARIES} MATCHES "NOTFOUND") )
  msg_error("Cannot find the required MPFR library!")
else()
  set(MPFR_FOUND ON)
endif()

check_pkg_version(result_ MPFR_VERSION_MAJOR MPFR_VERSION_MINOR
  MPFR_VERSION_PATCH "check_mpfr_version.cpp" ${MPFR_INCLUDE_DIRS} 
  ${MPFR_LIBRARIES})
if(result_)
  set(MPFR_VERSION 
    "${MPFR_VERSION_MAJOR}.${MPFR_VERSION_MINOR}.${MPFR_VERSION_PATCH}")
else()
  msg_error("Cannot determine version of MPFR library!\
 Please check installation of MPFR on this system.")
endif()

check_pkg_required_version(result_ "${MPFR_REQUIRED_VERSION}"
  ${MPFR_VERSION_MAJOR} ${MPFR_VERSION_MINOR} ${MPFR_VERSION_PATCH})
if(NOT result_)
  msg_error("Detected version of MPFR library is too old.\
  Required version is ${MPFR_REQUIRED_VERSION} (or newer).")
endif()

msg_status("MPFR version: " 
  "${MPFR_VERSION} (required ${MPFR_REQUIRED_VERSION})")
list(APPEND teuthid_link_libraries ${MPFR_LIBRARIES})
list_prepend(teuthid_INCLUDE_PATH ${MPFR_INCLUDE_DIRS})
list(REMOVE_DUPLICATES teuthid_INCLUDE_PATH)
