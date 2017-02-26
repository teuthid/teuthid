# GMP_FOUND - TRUE if GMP is available.
# GMP_VERSION - Version of GMP.
# GMP_INCLUDE_DIRS - Should be passed to target_include_directories() if
#                    the target is not used for linking.
# GMP_LIBRARIES - Can be passed to target_link_libraries() instead of GMP::GMP #                 target
# GMP::GMP - Imported target (if GMP_FOUND is TRUE).

set(GMP_REQUIRED_VERSION "6.1.1")

set(GMP_FOUND OFF)
find_path(GMP_INCLUDE_DIRS NAMES gmp.h)
find_library(GMP_LIBRARIES NAMES gmp libgmp)
if ( (${GMP_INCLUDE_DIRS} MATCHES "NOTFOUND") OR
    (${GMP_LIBRARIES} MATCHES "NOTFOUND") )
  msg_error("Cannot find the required GMP library!")
else()
  set(GMP_FOUND ON)
endif()

check_pkg_version(result_ GMP_VERSION_MAJOR GMP_VERSION_MINOR GMP_VERSION_PATCH
  "check_gmp_version.cpp" ${GMP_INCLUDE_DIRS} ${GMP_LIBRARIES})
if(result_)
  set(GMP_VERSION 
    "${GMP_VERSION_MAJOR}.${GMP_VERSION_MINOR}.${GMP_VERSION_PATCH}")
else()
  msg_error("Cannot determine version of GMP library!\
 Please check installation of GMP on this system.")
endif()

check_pkg_required_version(result_ "${GMP_REQUIRED_VERSION}"
  ${GMP_VERSION_MAJOR} ${GMP_VERSION_MINOR} ${GMP_VERSION_PATCH})
if(NOT result_)
  msg_error("Detected version of GMP library is too old.\
  Required version is ${GMP_REQUIRED_VERSION} (or newer).")
endif()


msg_status("GMP version: " "${GMP_VERSION} (required ${GMP_REQUIRED_VERSION})")
list(APPEND teuthid_link_libraries ${GMP_LIBRARIES})
list_prepend(teuthid_INCLUDE_PATH ${GMP_INCLUDE_DIRS})
list(REMOVE_DUPLICATES teuthid_INCLUDE_PATH)


#message("GMP_FOUND: ${GMP_FOUND}")
#message("GMP_VERSION: ${GMP_VERSION}")
#message("GMP_INCLUDE_DIRS: ${GMP_INCLUDE_DIRS}")
#message("GMP_LIBRARIES: ${GMP_LIBRARIES}")
