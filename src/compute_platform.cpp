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

#include <teuthid/compute_error.hpp>
#include <teuthid/compute_platform.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200
#include "cl2.hpp"
#endif // TEUTHID_WITH_OPENCL

using namespace teuthid;

std::mutex compute_platform::mutex_;
compute_platforms_t compute_platform::platforms_;

compute_platform::compute_platform(compute_platform_id_t id) {
  assert(id);
  bool __found = false;
  try {
  if (id && !compute_platform::platforms().empty())
    for (auto __platform : compute_platform::platforms_)
      if (id == __platform.id()) {
        *this = compute_platform(__platform);
        __found = true;
        break;
      }
  } catch (const compute_error&) {
    // something wrong with OpenCL, so a new object will be empty
  }
  if (!__found)
    *this = compute_platform();
}

bool compute_platform::is_required_version(int major, int minor) const
    noexcept {
  int __required = major * 100 + minor;
  int __actual = major_version_ * 100 + minor_version_;
  return (!(__required > __actual));
}

const compute_platforms_t &compute_platform::platforms() {
  std::lock_guard<std::mutex> __guard(mutex_);
  if (compute_platform::platforms_.empty()) {
    compute_platform::detect_platforms_();
    for (std::size_t __i = 0; __i < compute_platform::platforms_.size(); __i++)
      compute_platform::detect_devices_(compute_platform::platforms_[__i]);
  }
  return compute_platform::platforms_;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(TEUTHID_WITH_OPENCL)
compute_profile_t __teuthid_get_cl_profile(const std::string &str_profile) {
  if (str_profile == std::string("FULL_PROFILE"))
    return COMPUTE_FULL_PROFILE;
  else if (str_profile == std::string("EMBEDDED_PROFILE"))
    return COMPUTE_EMBEDDED_PROFILE;
  return COMPUTE_UNKNOWN_PROFILE;
}

void __teuthid_get_cl_version(const std::string &version, int &major,
                              int &minor, std::string &spec) {
  std::string __s = version.substr(7);
  std::string::size_type __dot = __s.find('.');
  assert(__dot != std::string::npos);
  major = std::stoi(__s.substr(0, __dot));
  assert(major != 0);
  std::string::size_type __space = __s.find(' ');
  assert(__space != std::string::npos);
  minor = std::stoi(__s.substr(__dot + 1, __space));
  spec = __s.substr(__space + 1);
}
#endif // TEUTHID_WITH_OPENCL
#endif // DOXYGEN_SHOULD_SKIP_THIS

void compute_platform::detect_platforms_() {
#if defined(TEUTHID_WITH_OPENCL)
  try {
    std::vector<cl::Platform> __cl_platforms;
    cl::Platform::get(&__cl_platforms);
    for (std::size_t __i = 0; __i < __cl_platforms.size(); __i++) {
      platforms_.push_back(compute_platform());
      platforms_[__i].id_ = __cl_platforms[__i]();
      assert(platforms_[__i].id_);
      platforms_[__i].profile_ = __teuthid_get_cl_profile(
          __cl_platforms[__i].getInfo<CL_PLATFORM_PROFILE>());
      platforms_[__i].version_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_VERSION>();
      __teuthid_get_cl_version(
          platforms_[__i].version_, platforms_[__i].major_version_,
          platforms_[__i].minor_version_, platforms_[__i].spec_version_);
      platforms_[__i].name_ = __cl_platforms[__i].getInfo<CL_PLATFORM_NAME>();
      platforms_[__i].vendor_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_VENDOR>();
      platforms_[__i].extensions_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_EXTENSIONS>();
      platforms_[__i].icd_suffix_khr_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_ICD_SUFFIX_KHR>();
    }
  } catch (const cl::Error &__e) {
    throw invalid_compute_platform(__e.err());
  }
#endif // TEUTHID_WITH_OPENCL
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(TEUTHID_WITH_OPENCL)
const compute_devtype_t __teuthid_get_cl_devtype(cl_device_type devtype) {
  if (devtype == CL_DEVICE_TYPE_CPU)
    return COMPUTE_DEVICE_CPU;
  else if (devtype == CL_DEVICE_TYPE_GPU)
    return COMPUTE_DEVICE_GPU;
  return COMPUTE_DEVICE_UNKNOWN;
}
#endif // TEUTHID_WITH_OPENCL
#endif // DOXYGEN_SHOULD_SKIP_THIS

void compute_platform::detect_devices_(compute_platform &platform) {
#if defined(TEUTHID_WITH_OPENCL)
  try {
    cl::Platform __cl_platform(platform.id_);
    std::vector<cl::Device> __cl_devices;
    __cl_platform.getDevices(CL_DEVICE_TYPE_ALL, &__cl_devices);
    for (std::size_t __i = 0; __i < __cl_devices.size(); __i++) {
      platform.devices_.push_back(compute_device());
      platform.devices_[__i].id_ = __cl_devices[__i]();
      platform.devices_[__i].profile_ = __teuthid_get_cl_profile(
          __cl_devices[__i].getInfo<CL_DEVICE_PROFILE>());
      platform.devices_[__i].name_ =
          __cl_devices[__i].getInfo<CL_DEVICE_NAME>();
      platform.devices_[__i].version_ =
          __cl_devices[__i].getInfo<CL_DEVICE_VERSION>();
      platform.devices_[__i].driver_version_ =
          __cl_devices[__i].getInfo<CL_DRIVER_VERSION>();
      platform.devices_[__i].c_version_ =
          __cl_devices[__i].getInfo<CL_DEVICE_OPENCL_C_VERSION>();
      platform.devices_[__i].max_compute_units_ =
          __cl_devices[__i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
      platform.devices_[__i].devtype_ =
          __teuthid_get_cl_devtype(__cl_devices[__i].getInfo<CL_DEVICE_TYPE>());
      platform.devices_[__i].extensions_ =
          __cl_devices[__i].getInfo<CL_DEVICE_EXTENSIONS>();
    }
  } catch (const cl::Error &__e) {
    throw invalid_compute_device(__e.err());
  }
#endif // TEUTHID_WITH_OPENCL
}
