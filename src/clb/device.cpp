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
#include <utility>

#include <teuthid/clb/error.hpp>
#include <teuthid/clb/platform.hpp>
#include <teuthid/system.hpp>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "cl_bindings.hpp"
#endif

using namespace teuthid::clb;

device::device(device_id_t device_id) {
  assert(device_id);
  *this = device::get(device_id);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
std::pair<const platform &, const device &>
__teuthid_clb_get(device_id_t device_id) {
  try {
    const platforms_t &__platforms = platform::get_all();
    for (std::size_t __i = 0; __i < __platforms.size(); __i++) {
      const devices_t &__devices = __platforms[__i].devices();
      for (std::size_t __j = 0; __j < __devices.size(); __j++)
        if (device_id == __devices[__j].id())
          return std::pair<const platform &, const device &>(__platforms[__i],
                                                             __devices[__j]);
    }
  } catch (const error &__e) {
    throw invalid_device(__e.cl_error());
  }
  throw invalid_device("unknown device_id_t");
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

const device &device::get(device_id_t device_id) {
  assert(device_id);
  return __teuthid_clb_get(device_id).second;
}

const device &device::get_default() {
  try {
    return device::get(cl::Device::getDefault()());
  } catch (const cl::Error &) {
    throw invalid_device("unknown a default device");
  }
}

const device &device::set_default(const device &dev) {
  try {
    cl::Device __dev = cl::Device::setDefault(cl::Device(dev.id()));
    return device::get(__dev());
  } catch (const cl::Error &) {
    throw invalid_device("unknown a default device");
  }
}

devices_t device::find_by_type(devtype_t dev_type) {
  cl_bitfield __devtype = static_cast<cl_bitfield>(dev_type);
  devices_t __devs;
  for (auto __platform : platform::get_all())
    for (auto __device : __platform.devices())
      if (static_cast<cl_bitfield>(__device.devtype()) & __devtype)
        __devs.push_back(__device);
  __devs.shrink_to_fit();
  return __devs;
}

const platform &device::get_platform(device_id_t device_id) {
  assert(device_id);
  return __teuthid_clb_get(device_id).first;
}

devices_t device::subdevices(const cl_device_partition_property *props) const {
  if (max_subdevices() < 2)
    throw invalid_device("cannot create subdevices");
  std::vector<cl::Device> __cl_subdevices;
  cl::Device __cl_device(id_);
  try {
    __cl_device.createSubDevices(props, &__cl_subdevices);
  } catch (const cl::Error &) {
    throw invalid_device("cannot create subdevices");
  }
  devices_t __devices;
  for (auto __dev : __cl_subdevices)
    __devices.push_back(device(__dev(), platform_id_));
  __devices.shrink_to_fit();
  return __devices;
}

devices_t device::subdevices_equally(std::size_t units) const {
  assert(max_subdevices() > 1);
  if (units < 1)
    throw invalid_device("invalid number of subdevices");
  cl_device_partition_property properties[] = {
      CL_DEVICE_PARTITION_EQUALLY,
      static_cast<cl_device_partition_property>(units), 0};
  return subdevices(properties);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __TEUTHID_CLB_DEVICE_INFO(PARAM)                                       \
  template <>                                                                  \
  device_param<devparam_t::PARAM>::value_type                                  \
  device::info<devparam_t::PARAM>() const {                                    \
    try {                                                                      \
      cl::Device __cl_device(id_);                                             \
      return static_cast<device_param<devparam_t::PARAM>::value_type>(         \
          __cl_device.getInfo<static_cast<cl_bitfield>(devparam_t::PARAM)>()); \
    } catch (const cl::Error &) {                                              \
      throw invalid_device("invalid device parameter");                        \
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
__TEUTHID_CLB_DEVICE_INFO(MAX_READ_IMAGE_ARGS);
__TEUTHID_CLB_DEVICE_INFO(MAX_SAMPLERS);
__TEUTHID_CLB_DEVICE_INFO(MAX_WORK_GROUP_SIZE);
__TEUTHID_CLB_DEVICE_INFO(MAX_WORK_ITEM_DIMENSIONS);
__TEUTHID_CLB_DEVICE_INFO(MAX_WORK_ITEM_SIZES);
__TEUTHID_CLB_DEVICE_INFO(MAX_WRITE_IMAGE_ARGS);
__TEUTHID_CLB_DEVICE_INFO(MEM_BASE_ADDR_ALIGN);
__TEUTHID_CLB_DEVICE_INFO(NAME);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_CHAR);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_SHORT);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_INT);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_LONG);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_FLOAT);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_DOUBLE);
__TEUTHID_CLB_DEVICE_INFO(NATIVE_VECTOR_WIDTH_HALF);
__TEUTHID_CLB_DEVICE_INFO(OPENCL_C_VERSION);
__TEUTHID_CLB_DEVICE_INFO(PARTITION_AFFINITY_DOMAIN);
__TEUTHID_CLB_DEVICE_INFO(PARTITION_PROPERTIES);
__TEUTHID_CLB_DEVICE_INFO(PARTITION_TYPE);
__TEUTHID_CLB_DEVICE_INFO(PIPE_MAX_ACTIVE_RESERVATIONS);
__TEUTHID_CLB_DEVICE_INFO(PIPE_MAX_PACKET_SIZE);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_GLOBAL_ATOMIC_ALIGNMENT);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_INTEROP_USER_SYNC);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_LOCAL_ATOMIC_ALIGNMENT);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_PLATFORM_ATOMIC_ALIGNMENT);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_CHAR);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_SHORT);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_INT);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_LONG);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_FLOAT);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_DOUBLE);
__TEUTHID_CLB_DEVICE_INFO(PREFERRED_VECTOR_WIDTH_HALF);
__TEUTHID_CLB_DEVICE_INFO(PROFILE);
__TEUTHID_CLB_DEVICE_INFO(PROFILING_TIMER_RESOLUTION);
__TEUTHID_CLB_DEVICE_INFO(QUEUE_ON_DEVICE_MAX_SIZE);
__TEUTHID_CLB_DEVICE_INFO(QUEUE_ON_DEVICE_PROPERTIES);
__TEUTHID_CLB_DEVICE_INFO(QUEUE_ON_HOST_PROPERTIES);
__TEUTHID_CLB_DEVICE_INFO(SINGLE_FP_CONFIG);
__TEUTHID_CLB_DEVICE_INFO(SVM_CAPABILITIES);
__TEUTHID_CLB_DEVICE_INFO(TYPE);
__TEUTHID_CLB_DEVICE_INFO(VENDOR);
__TEUTHID_CLB_DEVICE_INFO(VENDOR_ID);
__TEUTHID_CLB_DEVICE_INFO(VERSION);
__TEUTHID_CLB_DEVICE_INFO(DRIVER_VERSION);
#undef __TEUTHID_CLB_DEVICE_INFO

#define __TEUTHID_CLB_DEVICE_INFO(PARAM)                                       \
  template <>                                                                  \
  device_param<devparam_t::PARAM>::value_type                                  \
  device::info<devparam_t::PARAM>() const {                                    \
    typedef device_param<devparam_t::PARAM>::value_type __param_type;          \
    cl_int __result;                                                           \
    __param_type __param;                                                      \
    __result =                                                                 \
        clGetDeviceInfo(id_, static_cast<cl_bitfield>(devparam_t::PARAM),      \
                        sizeof(__param), &__param, NULL);                      \
    if (__result == CL_SUCCESS)                                                \
      return __param;                                                          \
    else if (__result == CL_INVALID_VALUE)                                     \
      throw invalid_device("invalid device parameter");                        \
    else                                                                       \
      throw invalid_device(__result);                                          \
  }

__TEUTHID_CLB_DEVICE_INFO(GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE);
__TEUTHID_CLB_DEVICE_INFO(IMAGE_BASE_ADDRESS_ALIGNMENT);
__TEUTHID_CLB_DEVICE_INFO(IMAGE_MAX_ARRAY_SIZE);
__TEUTHID_CLB_DEVICE_INFO(IMAGE_MAX_BUFFER_SIZE);
__TEUTHID_CLB_DEVICE_INFO(IMAGE_PITCH_ALIGNMENT);
__TEUTHID_CLB_DEVICE_INFO(MAX_GLOBAL_VARIABLE_SIZE);
__TEUTHID_CLB_DEVICE_INFO(MAX_NUM_SUB_GROUPS);
__TEUTHID_CLB_DEVICE_INFO(MAX_READ_WRITE_IMAGE_ARGS);
__TEUTHID_CLB_DEVICE_INFO(PARENT_DEVICE);
__TEUTHID_CLB_DEVICE_INFO(PARTITION_MAX_SUB_DEVICES);
__TEUTHID_CLB_DEVICE_INFO(PRINTF_BUFFER_SIZE);
#undef __TEUTHID_CLB_DEVICE_INFO
#endif // DOXYGEN_SHOULD_SKIP_THIS

device_id_t device::parent_id() const {
  return info<devparam_t::PARENT_DEVICE>();
}

uint32_t device::address_bits() const {
  return info<devparam_t::ADDRESS_BITS>();
}

bool device::is_available() const { return info<devparam_t::AVAILABLE>(); }

built_in_kernels_t device::built_in_kernels() const {
  built_in_kernels_t __v;
  system::split_string(info<devparam_t::BUILT_IN_KERNELS>(), __v, ';');
  return __v;
}

std::string device::c_version() const {
  return info<devparam_t::OPENCL_C_VERSION>().substr(9);
}

bool device::is_compiler_available() const {
  return info<devparam_t::COMPILER_AVAILABLE>();
}

devfp_config_t device::double_fp_config() const {
  return info<devparam_t::DOUBLE_FP_CONFIG>();
}

bool device::has_double_precision() const {
  cl_bitfield __dp =
      CL_FP_FMA | CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN | CL_FP_DENORM;
  return (static_cast<cl_bitfield>(double_fp_config()) & __dp) > 0;
}

extensions_t device::extensions() const {
  extensions_t __v;
  system::split_string(info<devparam_t::EXTENSIONS>(), __v);
  return __v;
}

bool device::has_extension(const std::string &ext_name) const {
  for (std::string __s : extensions())
    if (__s == ext_name)
      return true;
  return false;
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

uint32_t device::max_subdevices() const {
  return info<devparam_t::PARTITION_MAX_SUB_DEVICES>();
}

std::size_t device::max_work_group_size() const {
  return info<devparam_t::MAX_WORK_GROUP_SIZE>();
}

uint32_t device::max_work_item_dimensions() const {
  return info<devparam_t::MAX_WORK_ITEM_DIMENSIONS>();
}

std::string device::name() const { return info<devparam_t::NAME>(); }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(TYPE, PARAM)                  \
  template <> uint32_t device::native_vector_width<TYPE>() const {             \
    return info<devparam_t::PARAM>();                                          \
  }

__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(int8_t, NATIVE_VECTOR_WIDTH_CHAR);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(uint8_t, NATIVE_VECTOR_WIDTH_CHAR);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(int16_t, NATIVE_VECTOR_WIDTH_SHORT);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(uint16_t, NATIVE_VECTOR_WIDTH_SHORT);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(int32_t, NATIVE_VECTOR_WIDTH_INT);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(uint32_t, NATIVE_VECTOR_WIDTH_INT);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(int64_t, NATIVE_VECTOR_WIDTH_LONG);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(uint64_t, NATIVE_VECTOR_WIDTH_LONG);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(float16_t, NATIVE_VECTOR_WIDTH_HALF);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(float32_t, NATIVE_VECTOR_WIDTH_FLOAT);
__TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH(float64_t, NATIVE_VECTOR_WIDTH_DOUBLE);
#undef __TEUTHID_CLB_DEVICE_NATIVE_VECTOR_WIDTH

#define __TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(TYPE, PARAM)               \
  template <> uint32_t device::preferred_vector_width<TYPE>() const {          \
    return info<devparam_t::PARAM>();                                          \
  }

__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(int8_t,
                                            PREFERRED_VECTOR_WIDTH_CHAR);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(uint8_t,
                                            PREFERRED_VECTOR_WIDTH_CHAR);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(int16_t,
                                            PREFERRED_VECTOR_WIDTH_SHORT);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(uint16_t,
                                            PREFERRED_VECTOR_WIDTH_SHORT);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(int32_t,
                                            PREFERRED_VECTOR_WIDTH_INT);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(uint32_t,
                                            PREFERRED_VECTOR_WIDTH_INT);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(int64_t,
                                            PREFERRED_VECTOR_WIDTH_LONG);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(uint64_t,
                                            PREFERRED_VECTOR_WIDTH_LONG);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(float16_t,
                                            PREFERRED_VECTOR_WIDTH_HALF);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(float32_t,
                                            PREFERRED_VECTOR_WIDTH_FLOAT);
__TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH(float64_t,
                                            PREFERRED_VECTOR_WIDTH_DOUBLE);
#undef __TEUTHID_CLB_DEVICE_PREFERRED_VECTOR_WIDTH
#endif // DOXYGEN_SHOULD_SKIP_THIS

devprofile_t device::profile() const {
  std::string __s = info<devparam_t::PROFILE>();
  if (__s == std::string("FULL_PROFILE"))
    return devprofile_t::FULL;
  else if (__s == std::string("EMBEDDED_PROFILE"))
    return devprofile_t::EMBEDDED;
  else
    throw invalid_device("unknown device profile");
}

size_t device::profiling_timer_resolution() const {
  return info<devparam_t::PROFILING_TIMER_RESOLUTION>();
}

devfp_config_t device::single_fp_config() const {
  return info<devparam_t::SINGLE_FP_CONFIG>();
}

bool device::has_single_precision() const {
  cl_bitfield __dp = CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN;
  return (static_cast<cl_bitfield>(single_fp_config()) & __dp) > 0;
}

devtype_t device::devtype() const { return info<devparam_t::TYPE>(); }

bool device::is_devtype(devtype_t type_of_device) const {
  cl_bitfield __devtype = static_cast<cl_bitfield>(devtype());
  cl_bitfield __type = static_cast<cl_bitfield>(type_of_device);
  return (__devtype & __type) > 0;
}

std::string device::vendor() const { return info<devparam_t::VENDOR>(); }

std::string device::version() const {
  return info<devparam_t::VERSION>().substr(7);
}

std::string device::driver_version() const {
  return info<devparam_t::DRIVER_VERSION>();
}

bool device::check_version(int major, int minor) const {
  int act_major, act_minor;
  std::stringstream __s;
  __s << version();
  __s >> act_major;
  __s.ignore(1); // '.'
  __s >> act_minor;
  return (act_major > major || (act_major == major && act_minor >= minor));
}
