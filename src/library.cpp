/*
  This file is part of the Teuthid project.
  Copyright (C) 2016-2017 Mariusz Przygodzki (mariusz.przygodzki@gmail.com)

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <teuthid/library.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#endif

using namespace teuthid;

#if defined(TEUTHID_WITH_OPENCL)
bool library::have_opencl_ = library::check_opencl_();
#else
bool library::have_opencl_ = false;
#endif

thread_local bool library::use_opencl_ = library::have_opencl_;
std::mutex library::mutex_;
std::string library::version_ = std::string(TEUTHID_VERSION);
int library::num_cl_platforms_ = 0;
opencl_platforms_t library::opencl_platforms_ = opencl_platforms_t();

bool library::is_required_version(int min_major, int min_minor) {
  int __required = min_major * 1000 + min_minor;
  int __actual = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(__required > __actual));
}

bool library::use_opencl(bool enabled) {
  if (library::have_opencl_)
    library::use_opencl_ = enabled;
  return library::use_opencl_;
}

bool library::check_opencl_() {
#if defined(TEUTHID_WITH_OPENCL)
  cl_int __result;
  cl_uint __platform_count = 0;
  cl_platform_id *__platforms;

  __result = clGetPlatformIDs(0, NULL, &__platform_count);
  assert(__result == CL_SUCCESS);
  if (__result != CL_SUCCESS) // unable to get platform IDs
    return false;
  __platforms = new cl_platform_id[sizeof(cl_platform_id) * __platform_count];
  __result = clGetPlatformIDs(__platform_count, __platforms, NULL);
  assert(__result == CL_SUCCESS);
  if (__result != CL_SUCCESS) // unable to get platform IDs
    return false;
  else
    library::num_cl_platforms_ = __platform_count;

  char __data[1024];
  std::size_t __retsize;
  // platform queries
  for (cl_uint i = 0; i < __platform_count; i++) {
    opencl_platforms_.push_back(cl::platform_info());
    opencl_platforms_[i].id_ = i + 1; // id
    // profile
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_PROFILE,
                                 sizeof(__data), __data, &__retsize);
    assert(__result == CL_SUCCESS);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      continue; // unable to get platform's profile
    else {
      // ...
    }
    // ...
  }
  // ...

  delete __platforms;
  return true;
#else
  return false;
#endif // defined(TEUTHID_WITH_OPENCL)
}