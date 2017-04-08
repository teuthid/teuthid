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

#include <algorithm>
#include <cassert>

#include <teuthid/clb/error.hpp>
#include <teuthid/clb/platform.hpp>
#include <teuthid/system.hpp>

#include "cl_bindings.hpp"

using namespace teuthid;
using namespace teuthid::clb;

std::mutex platform::mutex_;
platforms_t platform::platforms_;

platform::platform(platform_id_t platform_id) {
  assert(platform_id);
  bool __found = false;
  try {
    if (platform::platform_count() > 0) {
      for (const platform &__platform : platform::platforms_)
        if (platform_id == __platform.id()) {
          *this = platform(__platform);
          __found = true;
          break;
        }
    }
  } catch (const error &__e) {
    throw invalid_platform(__e.cl_error());
  }
  if (!__found)
    throw invalid_platform("unknown platform_id_t");
}

bool platform::is_required_version(int major, int minor) const noexcept {
  int __required = major * 100 + minor;
  int __actual = platform::major_version_ * 100 + platform::minor_version_;
  return (!(__required > __actual));
}

bool platform::have_extension(const std::string &ext_name) const {
  if (!ext_name.empty())
    return std::find(platform::extensions_.begin(), platform::extensions_.end(),
                     ext_name) != platform::extensions_.end();
  return false;
}

const platforms_t &platform::platforms() {
  std::lock_guard<std::mutex> __guard(platform::mutex_);
  if (platform::platforms_.empty()) {
    platform::detect_platforms_();
    for (std::size_t __i = 0; __i < platform::platforms_.size(); __i++)
      platform::detect_devices_(platform::platforms_[__i]);
  }
  return platform::platforms_;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
profile_t __teuthid_get_cl_profile(const std::string &str_profile) {
  if (str_profile == std::string("FULL_PROFILE"))
    return profile_t::FULL_PROFILE;
  else if (str_profile == std::string("EMBEDDED_PROFILE"))
    return profile_t::EMBEDDED_PROFILE;
  return profile_t::UNKNOWN_PROFILE;
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
  minor = std::stoi(__s.substr(__dot + 1, __space - __dot));
  spec = __s.substr(__space + 1);
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

void platform::detect_platforms_() {
  try {
    std::vector<cl::Platform> __cl_platforms;
    cl::Platform::get(&__cl_platforms);
    for (std::size_t __i = 0; __i < __cl_platforms.size(); __i++) {
      platform::platforms_.push_back(platform());
      platform::platforms_[__i].id_ = __cl_platforms[__i]();
      assert(platform::platforms_[__i].id_);
      platform::platforms_[__i].profile_ = __teuthid_get_cl_profile(
          __cl_platforms[__i].getInfo<CL_PLATFORM_PROFILE>());
      platform::platforms_[__i].version_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_VERSION>();
      __teuthid_get_cl_version(platform::platforms_[__i].version_,
                               platform::platforms_[__i].major_version_,
                               platform::platforms_[__i].minor_version_,
                               platform::platforms_[__i].spec_version_);
      platform::platforms_[__i].name_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_NAME>();
      platform::platforms_[__i].vendor_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_VENDOR>();
      system::split_string(
          __cl_platforms[__i].getInfo<CL_PLATFORM_EXTENSIONS>(),
          platform::platforms_[__i].extensions_);
      platform::platforms_[__i].icd_suffix_khr_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_ICD_SUFFIX_KHR>();
    }
  } catch (const cl::Error &__e) {
    throw invalid_platform(__e.err());
  }
  platforms_.shrink_to_fit();
}

void platform::detect_devices_(platform &plat) {
  try {
    cl::Platform __cl_platform(plat.id_);
    std::vector<cl::Device> __cl_devices;
    __cl_platform.getDevices(CL_DEVICE_TYPE_ALL, &__cl_devices);
    for (std::size_t __i = 0; __i < __cl_devices.size(); __i++) {
      plat.devices_.push_back(device());
      plat.devices_[__i].id_ = __cl_devices[__i]();
      plat.devices_[__i].platform_id_ =
          __cl_devices[__i].getInfo<CL_DEVICE_PLATFORM>();
      plat.devices_[__i].profile_ = __teuthid_get_cl_profile(
          __cl_devices[__i].getInfo<CL_DEVICE_PROFILE>());
      plat.devices_[__i].name_ = __cl_devices[__i].getInfo<CL_DEVICE_NAME>();
      plat.devices_[__i].version_ =
          __cl_devices[__i].getInfo<CL_DEVICE_VERSION>();
      plat.devices_[__i].driver_version_ =
          __cl_devices[__i].getInfo<CL_DRIVER_VERSION>();
      plat.devices_[__i].c_version_ =
          __cl_devices[__i].getInfo<CL_DEVICE_OPENCL_C_VERSION>();
      plat.devices_[__i].max_compute_units_ =
          __cl_devices[__i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
      plat.devices_[__i].devtype_ =
          static_cast<devtype_t>(__cl_devices[__i].getInfo<CL_DEVICE_TYPE>());
      plat.devices_[__i].local_memory_size_ =
          __cl_devices[__i].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
    }
  } catch (const cl::Error &__e) {
    throw invalid_device(__e.err());
  }
  plat.devices_.shrink_to_fit();
}

bool platform::unload_compiler() {
  cl::Platform __cl_platform(id_);
  cl_int __result = __cl_platform.unloadCompiler();
  return (__result == CL_SUCCESS);
}
