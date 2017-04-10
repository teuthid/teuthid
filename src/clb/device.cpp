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

using namespace teuthid::clb;

device::device(device_id_t device_id) {
  assert(device_id);
  bool __found = false;
  try {
    const platform &__platform = device::get_platform(device_id);
    for (const device &__device : __platform.devices())
      if (device_id == __device.id()) {
        *this = device(__device);
        __found = true;
      }
  } catch (const error &__e) {
    throw invalid_device(__e.cl_error());
  }
  if (!__found)
    throw invalid_device("unknown device_id_t");
}

const platform &device::get_platform() const {
  try {
    const platforms_t &__platforms = platform::platforms();
    for (std::size_t __i = 0; __i < __platforms.size(); __i++)
      if (platform_id_ == __platforms[__i].id())
        return __platforms[__i];
  } catch (const error &__e) {
    throw invalid_platform(__e.cl_error());
  }
  throw invalid_platform("cannot find a platform");
}

const platform &device::get_platform(device_id_t device_id) {
  try {
    const platforms_t &__platforms = platform::platforms();
    for (std::size_t __i = 0; __i < __platforms.size(); __i++) {
      const devices_t &__devices = __platforms[__i].devices();
      for (std::size_t __j = 0; __j < __devices.size(); __j++)
        if (device_id == __devices[__j].id())
          return __platforms[__i];
    }
  } catch (const error &__e) {
    throw invalid_platform(__e.cl_error());
  }
  throw invalid_platform("cannot find a platform");
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define __TEUTHID_CLB_DEVICE_INFO(PARAM)                                       \
  template <>                                                                  \
  device_param<devparam_t::PARAM>::value_type                                  \
  device::info<devparam_t::PARAM>() const {                                    \
    try {                                                                      \
      cl::Device __cl_device(id_);                                             \
      return static_cast<device_param<devparam_t::PARAM>::value_type>(         \
          __cl_device.getInfo<static_cast<int>(devparam_t::PARAM)>());         \
    } catch (const cl::Error &__e) {                                           \
      throw invalid_device("unknown device parameter");                        \
    }                                                                          \
  }

__TEUTHID_CLB_DEVICE_INFO(ADDRESS_BITS);
__TEUTHID_CLB_DEVICE_INFO(AVAILABLE);
__TEUTHID_CLB_DEVICE_INFO(BUILT_IN_KERNELS);
__TEUTHID_CLB_DEVICE_INFO(COMPILER_AVAILABLE);
__TEUTHID_CLB_DEVICE_INFO(DOUBLE_FP_CONFIG);
__TEUTHID_CLB_DEVICE_INFO(ENDIAN_LITTLE);
__TEUTHID_CLB_DEVICE_INFO(ERROR_CORRECTION_SUPPORT);
__TEUTHID_CLB_DEVICE_INFO(EXECUTION_CAPABILITIES);
__TEUTHID_CLB_DEVICE_INFO(EXTENSIONS);
__TEUTHID_CLB_DEVICE_INFO(GLOBAL_MEM_CACHE_SIZE);
__TEUTHID_CLB_DEVICE_INFO(GLOBAL_MEM_CACHE_TYPE);
__TEUTHID_CLB_DEVICE_INFO(GLOBAL_MEM_CACHELINE_SIZE);
__TEUTHID_CLB_DEVICE_INFO(GLOBAL_MEM_SIZE);
__TEUTHID_CLB_DEVICE_INFO(IMAGE2D_MAX_HEIGHT);
__TEUTHID_CLB_DEVICE_INFO(IMAGE2D_MAX_WIDTH);
__TEUTHID_CLB_DEVICE_INFO(IMAGE3D_MAX_DEPTH);
__TEUTHID_CLB_DEVICE_INFO(IMAGE3D_MAX_HEIGHT);
__TEUTHID_CLB_DEVICE_INFO(IMAGE3D_MAX_WIDTH);
__TEUTHID_CLB_DEVICE_INFO(IMAGE_SUPPORT);
__TEUTHID_CLB_DEVICE_INFO(LOCAL_MEM_SIZE);
__TEUTHID_CLB_DEVICE_INFO(LOCAL_MEM_TYPE);
__TEUTHID_CLB_DEVICE_INFO(MAX_CLOCK_FREQUENCY);
__TEUTHID_CLB_DEVICE_INFO(MAX_COMPUTE_UNITS);
__TEUTHID_CLB_DEVICE_INFO(MAX_CONSTANT_ARGS);
__TEUTHID_CLB_DEVICE_INFO(MAX_CONSTANT_BUFFER_SIZE);
__TEUTHID_CLB_DEVICE_INFO(MAX_MEM_ALLOC_SIZE);
__TEUTHID_CLB_DEVICE_INFO(MAX_ON_DEVICE_EVENTS);
__TEUTHID_CLB_DEVICE_INFO(MAX_ON_DEVICE_QUEUES);
__TEUTHID_CLB_DEVICE_INFO(MAX_PARAMETER_SIZE);
__TEUTHID_CLB_DEVICE_INFO(MAX_PIPE_ARGS);
#undef __TEUTHID_CLB_DEVICE_INFO
#endif // DOXYGEN_SHOULD_SKIP_THIS

uint32_t device::address_bits() const {
  return info<devparam_t::ADDRESS_BITS>();
}

bool device::is_available() const { return info<devparam_t::AVAILABLE>(); }

built_in_kernels_t device::built_in_kernels() const {
  built_in_kernels_t __v;
  system::split_string(info<devparam_t::BUILT_IN_KERNELS>(), __v, ';');
  return __v;
}

bool device::is_compiler_available() const {
  return info<devparam_t::COMPILER_AVAILABLE>();
}

devfp_config_t device::double_fp_config() const {
  return info<devparam_t::DOUBLE_FP_CONFIG>();
}

bool device::have_double_precision() const {
  int __dp = CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN | CL_FP_DENORM;
  return (static_cast<int>(double_fp_config()) & __dp) > 0;
}

extensions_t device::extensions() const {
  extensions_t __v;
  system::split_string(info<devparam_t::EXTENSIONS>(), __v);
  return __v;
}

bool device::have_extension(const std::string &ext_name) const {
  std::string __s = info<devparam_t::EXTENSIONS>();
  return (__s.find(ext_name) != std::string::npos);
}

uint64_t device::global_mem_cache_size() const {
  return info<devparam_t::GLOBAL_MEM_CACHE_SIZE>();
}

uint32_t device::global_mem_cache_line_size() const {
  return info<devparam_t::GLOBAL_MEM_CACHELINE_SIZE>();
}

uint64_t device::global_mem_size() const {
  return info<devparam_t::GLOBAL_MEM_SIZE>();
}

uint64_t device::local_mem_size() const {
  return info<devparam_t::LOCAL_MEM_SIZE>();
}

uint32_t device::max_clock_frequency() const {
  return info<devparam_t::MAX_CLOCK_FREQUENCY>();
}

uint32_t device::max_compute_units() const {
  return info<devparam_t::MAX_COMPUTE_UNITS>();
}

uint64_t device::max_mem_alloc_size() const {
  return info<devparam_t::MAX_MEM_ALLOC_SIZE>();
}
