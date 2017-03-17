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

#include "cl_bindings.hpp"

using namespace teuthid::clb;

std::mutex platform::mutex_;
platforms_t platform::platforms_;

platform::platform(platform_id_t id) {
  assert(id);
  bool __found = false;
  try {
    if (id && !platform::platforms().empty())
      // will rewrite using std::find_if
      for (auto __platform : platform::platforms_)
        if (id == __platform.id()) {
          *this = platform(__platform);
          __found = true;
          break;
        }
  } catch (const error &) {
    // something wrong with OpenCL, so a new object will be empty
  }
  if (!__found)
    *this = platform();
}

bool platform::is_required_version(int major, int minor) const noexcept {
  int __required = major * 100 + minor;
  int __actual = major_version_ * 100 + minor_version_;
  return (!(__required > __actual));
}

bool platform::have_extension(const std::string &name) const {
  if (!name.empty())
    return std::find(extensions_.begin(), extensions_.end(), name) !=
           extensions_.end();
  return false;
}

const platforms_t &platform::platforms() {
  std::lock_guard<std::mutex> __guard(mutex_);
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
    return FULL_PROFILE;
  else if (str_profile == std::string("EMBEDDED_PROFILE"))
    return EMBEDDED_PROFILE;
  return UNKNOWN_PROFILE;
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

void __teuthid__get_cl_extensions(const std::string &str,
                                  extensions_t &extensions) {
  if (str.empty())
    return; // no extensions
  std::string __ex;
  std::string __s = str;
  std::string::size_type __space = __s.find(' ');
  if (__space == std::string::npos) // just one extension
    extensions.push_back(str);
  else {
    while (__space != std::string::npos) {
      __ex = __s.substr(0, __space);
      extensions.push_back(__ex);
      __s.erase(0, __ex.size() + 1);
      __space = __s.find(' ');
    }
    if (!__s.empty()) // last extension
      extensions.push_back(__s);
  }
  extensions.shrink_to_fit();
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

void platform::detect_platforms_() {
  try {
    std::vector<cl::Platform> __cl_platforms;
    cl::Platform::get(&__cl_platforms);
    for (std::size_t __i = 0; __i < __cl_platforms.size(); __i++) {
      platforms_.push_back(platform());
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
      __teuthid__get_cl_extensions(
          __cl_platforms[__i].getInfo<CL_PLATFORM_EXTENSIONS>(),
          platforms_[__i].extensions_);
      platforms_[__i].icd_suffix_khr_ =
          __cl_platforms[__i].getInfo<CL_PLATFORM_ICD_SUFFIX_KHR>();
    }
  } catch (const cl::Error &__e) {
    throw invalid_platform(__e.err());
  }
  platforms_.shrink_to_fit();
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
const devtype_t __teuthid_get_cl_devtype(cl_device_type devtype) {
  if (devtype == CL_DEVICE_TYPE_CPU)
    return DEVICE_CPU;
  else if (devtype == CL_DEVICE_TYPE_GPU)
    return DEVICE_GPU;
  return DEVICE_UNKNOWN;
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

void platform::detect_devices_(platform &plat) {
  try {
    cl::Platform __cl_platform(plat.id_);
    std::vector<cl::Device> __cl_devices;
    __cl_platform.getDevices(CL_DEVICE_TYPE_ALL, &__cl_devices);
    for (std::size_t __i = 0; __i < __cl_devices.size(); __i++) {
      plat.devices_.push_back(device());
      plat.devices_[__i].id_ = __cl_devices[__i]();
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
          __teuthid_get_cl_devtype(__cl_devices[__i].getInfo<CL_DEVICE_TYPE>());
      __teuthid__get_cl_extensions(
          __cl_devices[__i].getInfo<CL_DEVICE_EXTENSIONS>(),
          plat.devices_[__i].extensions_);
    }
  } catch (const cl::Error &__e) {
    throw invalid_device(__e.err());
  }
  plat.devices_.shrink_to_fit();
}
