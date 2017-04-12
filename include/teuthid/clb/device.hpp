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

#ifndef TEUTHID_CLB_DEVICE_HPP
#define TEUTHID_CLB_DEVICE_HPP

#include <string>
#include <vector>

#include <teuthid/config.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#endif

namespace teuthid {
namespace clb {

enum profile_t { FULL_PROFILE, EMBEDDED_PROFILE, UNKNOWN_PROFILE };

enum class devparam_t {
  ADDRESS_BITS = CL_DEVICE_ADDRESS_BITS,
  AVAILABLE = CL_DEVICE_AVAILABLE,
  BUILT_IN_KERNELS = CL_DEVICE_BUILT_IN_KERNELS,
  COMPILER_AVAILABLE = CL_DEVICE_COMPILER_AVAILABLE,
  DOUBLE_FP_CONFIG = CL_DEVICE_DOUBLE_FP_CONFIG,
  ENDIAN_LITTLE = CL_DEVICE_ENDIAN_LITTLE,
  ERROR_CORRECTION_SUPPORT = CL_DEVICE_ERROR_CORRECTION_SUPPORT,
  EXECUTION_CAPABILITIES = CL_DEVICE_EXECUTION_CAPABILITIES,
  EXTENSIONS = CL_DEVICE_EXTENSIONS,
  GLOBAL_MEM_CACHE_SIZE = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
  GLOBAL_MEM_CACHE_TYPE = CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
  GLOBAL_MEM_CACHELINE_SIZE = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
  GLOBAL_MEM_SIZE = CL_DEVICE_GLOBAL_MEM_SIZE,
  IMAGE2D_MAX_HEIGHT = CL_DEVICE_IMAGE2D_MAX_HEIGHT,
  IMAGE2D_MAX_WIDTH = CL_DEVICE_IMAGE2D_MAX_WIDTH,
  IMAGE3D_MAX_DEPTH = CL_DEVICE_IMAGE3D_MAX_DEPTH,
  IMAGE3D_MAX_HEIGHT = CL_DEVICE_IMAGE3D_MAX_HEIGHT,
  IMAGE3D_MAX_WIDTH = CL_DEVICE_IMAGE3D_MAX_WIDTH,
  IMAGE_SUPPORT = CL_DEVICE_IMAGE_SUPPORT,
  LOCAL_MEM_SIZE = CL_DEVICE_LOCAL_MEM_SIZE,
  LOCAL_MEM_TYPE = CL_DEVICE_LOCAL_MEM_TYPE,
  MAX_CLOCK_FREQUENCY = CL_DEVICE_MAX_CLOCK_FREQUENCY,
  MAX_COMPUTE_UNITS = CL_DEVICE_MAX_COMPUTE_UNITS,
  MAX_CONSTANT_ARGS = CL_DEVICE_MAX_CONSTANT_ARGS,
  MAX_CONSTANT_BUFFER_SIZE = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
  MAX_MEM_ALLOC_SIZE = CL_DEVICE_MAX_MEM_ALLOC_SIZE,
  MAX_ON_DEVICE_EVENTS = CL_DEVICE_MAX_ON_DEVICE_EVENTS,
  MAX_ON_DEVICE_QUEUES = CL_DEVICE_MAX_ON_DEVICE_QUEUES,
  MAX_PARAMETER_SIZE = CL_DEVICE_MAX_PARAMETER_SIZE,
  MAX_PIPE_ARGS = CL_DEVICE_MAX_PIPE_ARGS,
  MAX_READ_IMAGE_ARGS = CL_DEVICE_MAX_READ_IMAGE_ARGS,
  MAX_SAMPLERS = CL_DEVICE_MAX_SAMPLERS,
  MAX_WORK_GROUP_SIZE = CL_DEVICE_MAX_WORK_GROUP_SIZE,
  MAX_WORK_ITEM_DIMENSIONS = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
  MAX_WORK_ITEM_SIZES = CL_DEVICE_MAX_WORK_ITEM_SIZES,
  MAX_WRITE_IMAGE_ARGS = CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
  MEM_BASE_ADDR_ALIGN = CL_DEVICE_MEM_BASE_ADDR_ALIGN,
  NAME = CL_DEVICE_NAME,
  NATIVE_VECTOR_WIDTH_CHAR = CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR,
  NATIVE_VECTOR_WIDTH_SHORT = CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT,
  NATIVE_VECTOR_WIDTH_INT = CL_DEVICE_NATIVE_VECTOR_WIDTH_INT,
  NATIVE_VECTOR_WIDTH_LONG = CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG,
  NATIVE_VECTOR_WIDTH_FLOAT = CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT,
  NATIVE_VECTOR_WIDTH_DOUBLE = CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE,
  NATIVE_VECTOR_WIDTH_HALF = CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF,
  OPENCL_C_VERSION = CL_DEVICE_OPENCL_C_VERSION,
  PARTITION_AFFINITY_DOMAIN = CL_DEVICE_PARTITION_AFFINITY_DOMAIN,
  PARTITION_PROPERTIES = CL_DEVICE_PARTITION_PROPERTIES
  /* Not in cl2.hpp:
  CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE
  CL_DEVICE_IL_VERSION
  CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
  CL_DEVICE_IMAGE_MAX_ARRAY_SIZE
  CL_DEVICE_IMAGE_MAX_BUFFER_SIZE
  CL_DEVICE_IMAGE_PITCH_ALIGNMENT
  CL_DEVICE_LINKER_AVAILABLE
  CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE
  CL_DEVICE_MAX_NUM_SUB_GROUPS
  CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS
  CL_DEVICE_PARTITION_MAX_SUB_DEVICES
  */
};

enum class devaffinity_domain_t {
  NUMA = CL_DEVICE_AFFINITY_DOMAIN_NUMA,
  L4_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE,
  L3_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE,
  L2_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE,
  L1_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE,
  NEXT_PARTITIONABLE = CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE
};
enum class devexec_caps_t {
  KERNEL = CL_EXEC_KERNEL,
  NATIVE_KERNEL = CL_EXEC_NATIVE_KERNEL
};
enum class devfp_config_t {
  DENORM = CL_FP_DENORM,
  INF_NAN = CL_FP_INF_NAN,
  ROUND_TO_NEAREST = CL_FP_ROUND_TO_NEAREST,
  ROUND_TO_ZERO = CL_FP_ROUND_TO_ZERO,
  ROUND_TO_INF = CL_FP_ROUND_TO_INF,
  FMA = CL_FP_FMA,
  SOFT_FLOAT = CL_FP_SOFT_FLOAT,
  CORRECTLY_ROUNDED_DIVIDE_SQRT = CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT
};
enum class devlocal_mem_t {
  LOCAL = CL_LOCAL,
  GLOBAL = CL_GLOBAL,
  NONE = CL_NONE
};
enum class devmem_cache_t {
  NONE = CL_NONE,
  READ_ONLY_CACHE = CL_READ_ONLY_CACHE,
  READ_WRITE_CACHE = CL_READ_WRITE_CACHE
};
enum class devpartition_property_t {
  EQUALLY = CL_DEVICE_PARTITION_EQUALLY,
  BY_COUNTS = CL_DEVICE_PARTITION_BY_COUNTS,
  BY_AFFINITY_DOMAIN = CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN,
  NONE = 0
};
enum class devtype_t {
  CPU = CL_DEVICE_TYPE_CPU,
  GPU = CL_DEVICE_TYPE_GPU,
  ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
  CUSTOM = CL_DEVICE_TYPE_CUSTOM
};

template <devparam_t> struct device_param { typedef void value_type; };

class device;
class platform;

typedef cl_device_id device_id_t;
typedef cl_platform_id platform_id_t;
typedef std::vector<std::string> extensions_t;
typedef std::vector<std::string> built_in_kernels_t;
typedef std::vector<std::size_t> max_work_item_sizes_t;
typedef std::vector<intptr_t> partition_properties_t;
typedef std::vector<device> devices_t;
typedef std::vector<platform> platforms_t;

class device {
  friend class platform;

public:
  explicit device(device_id_t device_id);
  device(const device &) = default;
  device(device &&) = default;
  virtual ~device() {}
  device &operator=(const device &) = default;
  device &operator=(device &&) = default;

  template <devparam_t value>
  typename device_param<value>::value_type info() const;

  const device_id_t &id() const noexcept { return id_; }
  const platform &get_platform() const;
  const profile_t &profile() const noexcept { return profile_; }
  bool is_full_profile() const noexcept {
    return (profile_ == profile_t::FULL_PROFILE);
  }
  bool is_embedded_profile() const noexcept {
    return (profile_ == profile_t::EMBEDDED_PROFILE);
  }
  const devtype_t &devtype() const noexcept { return devtype_; }
  bool is_devtype_cpu() const noexcept { return (devtype_ == devtype_t::CPU); }
  bool is_devtype_gpu() const noexcept { return (devtype_ == devtype_t::GPU); }
  bool is_devtype_accelerator() const noexcept {
    return (devtype_ == devtype_t::ACCELERATOR);
  }
  const std::string &version() const noexcept { return version_; }
  const std::string &driver_version() const noexcept { return driver_version_; }

  uint32_t address_bits() const;
  bool is_available() const;
  built_in_kernels_t built_in_kernels() const;
  bool is_compiler_available() const;
  devfp_config_t double_fp_config() const;
  bool have_double_precision() const;
  extensions_t extensions() const;
  bool have_extension(const std::string &ext_name) const;
  uint64_t global_mem_cache_size() const;
  uint32_t global_mem_cache_line_size() const;
  uint64_t global_mem_size() const;
  uint64_t local_mem_size() const;
  uint32_t max_clock_frequency() const;
  uint32_t max_compute_units() const;
  uint64_t max_mem_alloc_size() const;
  std::size_t max_work_group_size() const;
  uint32_t max_work_item_dimensions() const;
  std::string name() const;
  template <typename T> uint32_t native_vector_width() const { return 0; }
  std::string c_version() const;

  bool operator==(const device &other) const { return id_ == other.id_; }
  bool operator!=(const device &other) const { return id_ != other.id_; }

  static const platform &get_platform(device_id_t device_id);

private:
  device() {}

  device_id_t id_;             // device ID
  platform_id_t platform_id_;  // platform ID
  profile_t profile_;          // CL_DEVICE_PROFILE
  devtype_t devtype_;          // CL_DEVICE_TYPE
  std::string version_;        // CL_DEVICE_VERSION
  std::string driver_version_; // CL_DRIVER_VERSION
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// specialization of device::info<>()
#define __TEUTHID_CLB_DEVICE_INFO_SPEC(PARAM, VALUE_TYPE)                      \
  template <> struct device_param<devparam_t::PARAM> {                         \
    typedef VALUE_TYPE value_type;                                             \
  };                                                                           \
  template <>                                                                  \
  device_param<devparam_t::PARAM>::value_type                                  \
  device::info<devparam_t::PARAM>() const;

__TEUTHID_CLB_DEVICE_INFO_SPEC(ADDRESS_BITS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(AVAILABLE, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(BUILT_IN_KERNELS, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(COMPILER_AVAILABLE, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(DOUBLE_FP_CONFIG, devfp_config_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(ENDIAN_LITTLE, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(ERROR_CORRECTION_SUPPORT, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(EXECUTION_CAPABILITIES, devexec_caps_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(EXTENSIONS, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(GLOBAL_MEM_CACHE_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(GLOBAL_MEM_CACHE_TYPE, devmem_cache_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(GLOBAL_MEM_CACHELINE_SIZE, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(GLOBAL_MEM_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE2D_MAX_HEIGHT, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE2D_MAX_WIDTH, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE3D_MAX_DEPTH, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE3D_MAX_HEIGHT, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE3D_MAX_WIDTH, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE_SUPPORT, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(LOCAL_MEM_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(LOCAL_MEM_TYPE, devlocal_mem_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_CLOCK_FREQUENCY, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_COMPUTE_UNITS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_CONSTANT_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_CONSTANT_BUFFER_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_MEM_ALLOC_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_ON_DEVICE_EVENTS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_ON_DEVICE_QUEUES, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_PARAMETER_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_PIPE_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_READ_IMAGE_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_SAMPLERS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_WORK_GROUP_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_WORK_ITEM_DIMENSIONS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_WORK_ITEM_SIZES, max_work_item_sizes_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_WRITE_IMAGE_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MEM_BASE_ADDR_ALIGN, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NAME, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_CHAR, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_SHORT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_INT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_LONG, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_FLOAT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_DOUBLE, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(NATIVE_VECTOR_WIDTH_HALF, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(OPENCL_C_VERSION, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PARTITION_AFFINITY_DOMAIN, devaffinity_domain_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PARTITION_PROPERTIES, partition_properties_t)
#undef __TEUTHID_CLB_DEVICE_INFO_SPEC

// specialization of device::native_vector_width<>()
template <> uint32_t device::native_vector_width<int8_t>() const;
template <> uint32_t device::native_vector_width<uint8_t>() const;
template <> uint32_t device::native_vector_width<int16_t>() const;
template <> uint32_t device::native_vector_width<uint16_t>() const;
template <> uint32_t device::native_vector_width<int32_t>() const;
template <> uint32_t device::native_vector_width<uint32_t>() const;
template <> uint32_t device::native_vector_width<int64_t>() const;
template <> uint32_t device::native_vector_width<uint64_t>() const;
template <> uint32_t device::native_vector_width<float16_t>() const;
template <> uint32_t device::native_vector_width<float32_t>() const;
template <> uint32_t device::native_vector_width<float64_t>() const;
#endif // DOXYGEN_SHOULD_SKIP_THIS
} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_DEVICE_HPP