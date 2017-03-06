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
    along with the Teuthid.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <memory>

#include <teuthid/cl_platform_info.hpp>

using namespace teuthid;
using namespace teuthid::cl;

std::mutex platform_info::mutex_;
bool platform_info::platforms_detected_ = false;
opencl_platforms_t platform_info::platforms_ = opencl_platforms_t();

bool platform_info::is_required_version(int major, int minor) const {
  int __required = major * 100 + minor;
  int __actual = major_version_ * 100 + minor_version_;
  return (!(__required > __actual));
}

const opencl_platforms_t &platform_info::platforms(bool force_detection) {
  std::lock_guard<std::mutex> __guard(mutex_);
  if (!platform_info::platforms_detected_ || force_detection) {
    platform_info::detect_platforms_();
    platform_info::platforms_detected_ = true;
  }
  return platform_info::platforms_;
}

void platform_info::detect_platforms_() {
  platform_info::platforms_.clear();
#if defined(TEUTHID_WITH_OPENCL)
  cl_int __result;
  cl_uint __platform_count = 0;
  cl_platform_id *__platforms;
  char __data[1024];
  std::size_t __retsize;
  std::string __str;

  __result = clGetPlatformIDs(0, NULL, &__platform_count);
  assert(__result == CL_SUCCESS);
  if (__result != CL_SUCCESS) // unable to get platform IDs
    return;
  __platforms = new cl_platform_id[sizeof(cl_platform_id) * __platform_count];
  std::unique_ptr<cl_platform_id> __uptr(__platforms);
  __result = clGetPlatformIDs(__platform_count, __platforms, NULL);
  assert(__result == CL_SUCCESS);
  if (__result != CL_SUCCESS) // unable to get platform IDs
    return;

  // platform queries
  for (cl_int i = 0; i < __platform_count; i++) {
    assert(__platforms[i]);
    platforms_.push_back(platform_info());
    platforms_[i].id_ = __platforms[i];
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_PROFILE,
                                 sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data))) {
      platforms_[i].profile_ = UNKNOWN_PROFILE;
      continue; // unable to get platform's profile
    }
    __str = std::string(__data);
    if (__str == std::string("FULL_PROFILE"))
      platforms_[i].profile_ = FULL_PROFILE;
    else if (__str == std::string("EMBEDDED_PROFILE"))
      platforms_[i].profile_ = EMBEDDED_PROFILE;
    else
      platforms_[i].profile_ = UNKNOWN_PROFILE;
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_VERSION,
                                 sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      assert(false);
    else {
      __str = std::string(__data);
      __str = __str.substr(7);
      assert(!__str.empty());
      platforms_[i].version_ = __str;
      std::string::size_type __dot = __str.find('.');
      assert(__dot != std::string::npos);
      std::string::size_type __space = __str.find(' ');
      assert(__space != std::string::npos);
      platforms_[i].major_version_ = std::stoi(__str.substr(0, __dot));
      platforms_[i].minor_version_ =
          std::stoi(__str.substr(__dot + 1, __space));
      platforms_[i].spec_version_ = __str.substr(__space + 1);
    }
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_NAME,
                                 sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      assert(false);
    else
      platforms_[i].name_ = std::string(__data);
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_VENDOR,
                                 sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      assert(false);
    else
      platforms_[i].vendor_ = std::string(__data);
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_EXTENSIONS,
                                 sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      ; // no vendor ?!
    else
      platforms_[i].extensions_ = std::string(__data);
    __result =
        clGetPlatformInfo(__platforms[i], CL_PLATFORM_HOST_TIMER_RESOLUTION,
                          sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      ; // not supported
    else
      platforms_[i].host_timer_resolution_ = std::stoull(__data);
    __result = clGetPlatformInfo(__platforms[i], CL_PLATFORM_ICD_SUFFIX_KHR,
                                 sizeof(__data), __data, &__retsize);
    if ((__result != CL_SUCCESS) || (__retsize == sizeof(__data)))
      ; // not supported
    else
      platforms_[i].icd_suffix_khr_ = std::string(__data);
  }
#endif // defined(TEUTHID_WITH_OPENCL)
}
