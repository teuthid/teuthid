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
#include <sstream>

#include <teuthid/clb/error.hpp>
#include <teuthid/clb/platform.hpp>
#include <teuthid/system.hpp>

#include "cl_bindings.hpp"

using namespace teuthid;
using namespace teuthid::clb;

std::mutex platform::mutex_;
platforms_t platform::platforms_;

const platform &platform::get(platform_id_t platform_id) {
  assert(platform_id);
  try {
    if (platform::count() > 0) {
      for (const platform &__platform : platform::platforms_)
        if (platform_id == __platform.id())
          return __platform;
    }
  } catch (const error &__e) {
    throw invalid_platform(__e.cl_error());
  }
  throw invalid_platform("invalid platform_id_t");
}

const platforms_t &platform::get_all() {
  std::lock_guard<std::mutex> __guard(platform::mutex_);
  if (platform::platforms_.empty()) {
    platform::detect_platforms_();
    for (std::size_t __i = 0; __i < platform::platforms_.size(); __i++)
      platform::detect_devices_(platform::platforms_[__i]);
  }
  return platform::platforms_;
}

const platform &platform::get_default() {
  try {
    return platform::get(cl::Platform::getDefault()());
  } catch (const cl::Error &) {
    throw invalid_platform("invalid default platform");
  }
}

const platform &platform::set_default(const platform &plat) {
  try {
    cl::Platform __plat = cl::Platform::setDefault(cl::Platform(plat.id()));
    return platform::get(__plat());
  } catch (const cl::Error &) {
    throw invalid_platform("invalid default platform");
  }
}

void platform::detect_platforms_() {
  try {
    std::vector<cl::Platform> __cl_platforms;
    cl::Platform::get(&__cl_platforms);
    for (std::size_t __i = 0; __i < __cl_platforms.size(); __i++) {
      platform::platforms_.push_back(platform());
      platform::platforms_[__i].id_ = __cl_platforms[__i]();
      assert(platform::platforms_[__i].id_);
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
    for (std::size_t __i = 0; __i < __cl_devices.size(); __i++)
      plat.devices_.push_back(device(__cl_devices[__i](), nullptr, plat.id_));
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __TEUTHID_CLB_PLATFORM_INFO(PARAM)                                     \
  template <>                                                                  \
  platform_param<platparam_t::PARAM>::value_type                               \
  platform::info<platparam_t::PARAM>() const {                                 \
    try {                                                                      \
      cl::Platform __cl_plattform(id_);                                        \
      return static_cast<platform_param<platparam_t::PARAM>::value_type>(      \
          __cl_plattform                                                       \
              .getInfo<static_cast<cl_bitfield>(platparam_t::PARAM)>());       \
    } catch (const cl::Error &) {                                              \
      throw invalid_platform("invalid platform parameter");                    \
    }                                                                          \
  }

__TEUTHID_CLB_PLATFORM_INFO(PROFILE);
__TEUTHID_CLB_PLATFORM_INFO(VERSION);
__TEUTHID_CLB_PLATFORM_INFO(NAME);
__TEUTHID_CLB_PLATFORM_INFO(VENDOR);
__TEUTHID_CLB_PLATFORM_INFO(EXTENSIONS);
__TEUTHID_CLB_PLATFORM_INFO(ICD_SUFFIX_KHR);
#undef __TEUTHID_CLB_PLATFORM_INFO

#define __TEUTHID_CLB_PLATFORM_INFO(PARAM)                                     \
  template <>                                                                  \
  platform_param<platparam_t::PARAM>::value_type                               \
  platform::info<platparam_t::PARAM>() const {                                 \
    typedef platform_param<platparam_t::PARAM>::value_type __param_type;       \
    cl_int __result;                                                           \
    __param_type __param;                                                      \
    __result =                                                                 \
        clGetPlatformInfo(id_, static_cast<cl_bitfield>(platparam_t::PARAM),   \
                          sizeof(__param), &__param, NULL);                    \
    if (__result == CL_SUCCESS)                                                \
      return __param;                                                          \
    else if (__result == CL_INVALID_VALUE)                                     \
      throw invalid_platform("invalid platform parameter");                    \
    else                                                                       \
      throw invalid_platform(__result);                                        \
  }

__TEUTHID_CLB_PLATFORM_INFO(HOST_TIMER_RESOLUTION);
#undef __TEUTHID_CLB_PLATFORM_INFO
#endif // DOXYGEN_SHOULD_SKIP_THIS

platprofile_t platform::profile() const {
  std::string __s = info<platparam_t::PROFILE>();
  if (__s == std::string("FULL_PROFILE"))
    return platprofile_t::FULL;
  else if (__s == std::string("EMBEDDED_PROFILE"))
    return platprofile_t::EMBEDDED;
  else
    throw invalid_platform("invalid platform profile");
}

std::string platform::version() const {
  return info<platparam_t::VERSION>().substr(7);
}

bool platform::check_version(int major, int minor) const {
  int act_major, act_minor;
  std::stringstream __s;
  __s << version();
  __s >> act_major;
  __s.ignore(1); // '.'
  __s >> act_minor;
  return (act_major > major || (act_major == major && act_minor >= minor));
}

std::string platform::name() const { return info<platparam_t::NAME>(); }

std::string platform::vendor() const { return info<platparam_t::VENDOR>(); }

extensions_t platform::extensions() const {
  extensions_t __v;
  system::split_string(info<platparam_t::EXTENSIONS>(), __v);
  return __v;
}

bool platform::has_extension(const std::string &ext_name) const {
  for (std::string __s : extensions())
    if (__s == ext_name)
      return true;
  return false;
}

uint64_t platform::host_timer_resolution() const {
  return info<platparam_t::HOST_TIMER_RESOLUTION>();
}

std::string platform::icd_suffix_khr() const {
  return info<platparam_t::ICD_SUFFIX_KHR>();
}
