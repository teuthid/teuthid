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

#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

namespace teuthid {
namespace clb {

enum class devparam_t : uint64_t { // cl_device_info
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
  GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE =
      CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE,
  IMAGE2D_MAX_HEIGHT = CL_DEVICE_IMAGE2D_MAX_HEIGHT,
  IMAGE2D_MAX_WIDTH = CL_DEVICE_IMAGE2D_MAX_WIDTH,
  IMAGE3D_MAX_DEPTH = CL_DEVICE_IMAGE3D_MAX_DEPTH,
  IMAGE3D_MAX_HEIGHT = CL_DEVICE_IMAGE3D_MAX_HEIGHT,
  IMAGE3D_MAX_WIDTH = CL_DEVICE_IMAGE3D_MAX_WIDTH,
  IMAGE_BASE_ADDRESS_ALIGNMENT = CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT,
  IMAGE_MAX_ARRAY_SIZE = CL_DEVICE_IMAGE_MAX_ARRAY_SIZE,
  IMAGE_MAX_BUFFER_SIZE = CL_DEVICE_IMAGE_MAX_BUFFER_SIZE,
  IMAGE_PITCH_ALIGNMENT = CL_DEVICE_IMAGE_PITCH_ALIGNMENT,
  IMAGE_SUPPORT = CL_DEVICE_IMAGE_SUPPORT,
  LOCAL_MEM_SIZE = CL_DEVICE_LOCAL_MEM_SIZE,
  LOCAL_MEM_TYPE = CL_DEVICE_LOCAL_MEM_TYPE,
  MAX_CLOCK_FREQUENCY = CL_DEVICE_MAX_CLOCK_FREQUENCY,
  MAX_COMPUTE_UNITS = CL_DEVICE_MAX_COMPUTE_UNITS,
  MAX_CONSTANT_ARGS = CL_DEVICE_MAX_CONSTANT_ARGS,
  MAX_CONSTANT_BUFFER_SIZE = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
  MAX_GLOBAL_VARIABLE_SIZE = CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE,
  MAX_MEM_ALLOC_SIZE = CL_DEVICE_MAX_MEM_ALLOC_SIZE,
  MAX_NUM_SUB_GROUPS = CL_DEVICE_MAX_NUM_SUB_GROUPS,
  MAX_ON_DEVICE_EVENTS = CL_DEVICE_MAX_ON_DEVICE_EVENTS,
  MAX_ON_DEVICE_QUEUES = CL_DEVICE_MAX_ON_DEVICE_QUEUES,
  MAX_PARAMETER_SIZE = CL_DEVICE_MAX_PARAMETER_SIZE,
  MAX_PIPE_ARGS = CL_DEVICE_MAX_PIPE_ARGS,
  MAX_READ_IMAGE_ARGS = CL_DEVICE_MAX_READ_IMAGE_ARGS,
  MAX_READ_WRITE_IMAGE_ARGS = CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS,
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
  PARTITION_MAX_SUB_DEVICES = CL_DEVICE_PARTITION_MAX_SUB_DEVICES,
  PARTITION_PROPERTIES = CL_DEVICE_PARTITION_PROPERTIES,
  PARTITION_TYPE = CL_DEVICE_PARTITION_TYPE,
  PIPE_MAX_ACTIVE_RESERVATIONS = CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS,
  PIPE_MAX_PACKET_SIZE = CL_DEVICE_PIPE_MAX_PACKET_SIZE,
  PREFERRED_GLOBAL_ATOMIC_ALIGNMENT =
      CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT,
  PREFERRED_INTEROP_USER_SYNC = CL_DEVICE_PREFERRED_INTEROP_USER_SYNC,
  PREFERRED_LOCAL_ATOMIC_ALIGNMENT = CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT,
  PREFERRED_PLATFORM_ATOMIC_ALIGNMENT =
      CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT,
  PREFERRED_VECTOR_WIDTH_CHAR = CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
  PREFERRED_VECTOR_WIDTH_SHORT = CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
  PREFERRED_VECTOR_WIDTH_INT = CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
  PREFERRED_VECTOR_WIDTH_LONG = CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
  PREFERRED_VECTOR_WIDTH_FLOAT = CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
  PREFERRED_VECTOR_WIDTH_DOUBLE = CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
  PREFERRED_VECTOR_WIDTH_HALF = CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF,
  PRINTF_BUFFER_SIZE = CL_DEVICE_PRINTF_BUFFER_SIZE,
  PROFILE = CL_DEVICE_PROFILE,
  PROFILING_TIMER_RESOLUTION = CL_DEVICE_PROFILING_TIMER_RESOLUTION,
  QUEUE_ON_DEVICE_MAX_SIZE = CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE,
  QUEUE_ON_DEVICE_PREFERRED_SIZE = CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE,
  QUEUE_ON_DEVICE_PROPERTIES = CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES,
  QUEUE_ON_HOST_PROPERTIES = CL_DEVICE_QUEUE_ON_HOST_PROPERTIES,
  REFERENCE_COUNT = CL_DEVICE_REFERENCE_COUNT,
  SINGLE_FP_CONFIG = CL_DEVICE_SINGLE_FP_CONFIG,
  SVM_CAPABILITIES = CL_DEVICE_SVM_CAPABILITIES,
  TYPE = CL_DEVICE_TYPE,
  VENDOR = CL_DEVICE_VENDOR,
  VENDOR_ID = CL_DEVICE_VENDOR_ID,
  VERSION = CL_DEVICE_VERSION,
  DRIVER_VERSION = CL_DRIVER_VERSION
  // CL_DEVICE_IL_VERSION
  // CL_DEVICE_LINKER_AVAILABLE
  // CL_DEVICE_SPIR_VERSIONS
  // CL_DEVICE_SUBGROUP_INDEPENDENT_FORWARD_PROGRESS
  // CL_DEVICE_TERMINATE_CAPABILITY_KHR
};

enum class devaffinity_domain_t : uint64_t { // cl_device_affinity_domain
  NUMA = CL_DEVICE_AFFINITY_DOMAIN_NUMA,
  L4_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE,
  L3_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE,
  L2_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE,
  L1_CACHE = CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE,
  NEXT_PARTITIONABLE = CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE
};
enum class
    devcommand_queue_properties_t : uint64_t { // cl_command_queue_properties
      OUT_OF_ORDER_EXEC_MODE_ENABLE = CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
      PROFILING_ENABLE = CL_QUEUE_PROFILING_ENABLE,
      ON_DEVICE = CL_QUEUE_ON_DEVICE,
      ON_DEVICE_DEFAULT = CL_QUEUE_ON_DEVICE_DEFAULT
    };
enum class devexec_caps_t : uint64_t { // cl_device_exec_capabilities
  KERNEL = CL_EXEC_KERNEL,
  NATIVE_KERNEL = CL_EXEC_NATIVE_KERNEL
};
enum class devfp_config_t : uint64_t { // cl_device_fp_config
  DENORM = CL_FP_DENORM,
  INF_NAN = CL_FP_INF_NAN,
  ROUND_TO_NEAREST = CL_FP_ROUND_TO_NEAREST,
  ROUND_TO_ZERO = CL_FP_ROUND_TO_ZERO,
  ROUND_TO_INF = CL_FP_ROUND_TO_INF,
  FMA = CL_FP_FMA,
  SOFT_FLOAT = CL_FP_SOFT_FLOAT,
  CORRECTLY_ROUNDED_DIVIDE_SQRT = CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT
};
enum class devlocal_mem_t : uint64_t { // cl_device_local_mem_type
  LOCAL = CL_LOCAL,
  GLOBAL = CL_GLOBAL,
  NONE = CL_NONE ///< No local memory supported.
};
enum class devmem_cache_t : uint64_t { // cl_device_mem_cache_type
  READ_ONLY_CACHE = CL_READ_ONLY_CACHE,
  READ_WRITE_CACHE = CL_READ_WRITE_CACHE,
  NONE = CL_NONE ///< No global memory cache supported.
};
enum class devpartition_property_t : uint64_t { // cl_device_partition_property
  EQUALLY = CL_DEVICE_PARTITION_EQUALLY,
  BY_COUNTS = CL_DEVICE_PARTITION_BY_COUNTS,
  BY_AFFINITY_DOMAIN = CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN
};
enum class devprofile_t { FULL, EMBEDDED };
enum class devsvm_capabilities_t : uint64_t { // cl_device_svm_capabilities
  COARSE_GRAIN_BUFFER = CL_DEVICE_SVM_COARSE_GRAIN_BUFFER,
  FINE_GRAIN_BUFFER = CL_DEVICE_SVM_FINE_GRAIN_BUFFER,
  FINE_GRAIN_SYSTEM = CL_DEVICE_SVM_FINE_GRAIN_SYSTEM,
  ATOMICS = CL_DEVICE_SVM_ATOMICS
};
enum class devtype_t : uint64_t { // cl_device_type
  CPU = CL_DEVICE_TYPE_CPU,
  GPU = CL_DEVICE_TYPE_GPU,
  ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
  CUSTOM = CL_DEVICE_TYPE_CUSTOM,
  DEFAULT = CL_DEVICE_TYPE_DEFAULT,
  ALL = CL_DEVICE_TYPE_ALL
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

  device_id_t id() const noexcept { return id_; }
  device_id_t parent_id() const noexcept { return parent_id_; }
  bool is_subdevice() const noexcept { return (parent_id_ != nullptr); }
  devices_t subdevices_equally(std::size_t units) const;
  const platform &get_platform() const { return device::get_platform(id_); }

  uint32_t address_bits() const;
  bool is_available() const;
  built_in_kernels_t built_in_kernels() const;
  std::string c_version() const;
  bool is_compiler_available() const;
  devfp_config_t double_fp_config() const;
  bool has_double_precision() const;
  extensions_t extensions() const;
  bool has_extension(const std::string &ext_name) const;
  uint64_t global_mem_cache_size() const;
  uint32_t global_mem_cache_line_size() const;
  uint64_t global_mem_size() const;
  uint64_t local_mem_size() const;
  uint32_t max_clock_frequency() const;
  uint32_t max_compute_units() const;
  uint64_t max_mem_alloc_size() const;
  uint32_t max_subdevices() const;
  std::size_t max_work_group_size() const;
  uint32_t max_work_item_dimensions() const;
  std::string name() const;
  template <typename T> uint32_t native_vector_width() const { return 0; }
  template <typename T> uint32_t preferred_vector_width() const { return 0; }
  devprofile_t profile() const;
  bool is_full_profile() const { return (profile() == devprofile_t::FULL); }
  bool is_embedded_profile() const {
    return (profile() == devprofile_t::EMBEDDED);
  }
  size_t profiling_timer_resolution() const;
  devfp_config_t single_fp_config() const;
  bool has_single_precision() const;
  devtype_t devtype() const;
  bool is_devtype(devtype_t type_of_device) const;
  bool is_cpu() const { return is_devtype(devtype_t::CPU); }
  bool is_gpu() const { return is_devtype(devtype_t::GPU); }
  std::string vendor() const;
  std::string version() const;
  std::string driver_version() const;
  bool check_version(int major, int minor) const;

  bool operator==(const device &other) const { return id_ == other.id_; }
  bool operator!=(const device &other) const { return id_ != other.id_; }

  static const device &get(device_id_t device_id);
  static const device &get_default();
  static const device &set_default(const device &dev);
  static devices_t find_by_type(devtype_t dev_type);
  static const platform &get_platform(device_id_t device_id);

protected:
  devices_t subdevices(const cl_device_partition_property *props) const;

private:
  device() {}
  explicit device(device_id_t device_id, device_id_t parent_id,
                  platform_id_t platform_id)
      : id_(device_id), parent_id_(parent_id), platform_id_(platform_id) {}
  device_id_t id_;            // device ID
  device_id_t parent_id_;     // parent device ID
  platform_id_t platform_id_; // platform ID
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
__TEUTHID_CLB_DEVICE_INFO_SPEC(GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE,
                               std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE2D_MAX_HEIGHT, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE2D_MAX_WIDTH, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE3D_MAX_DEPTH, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE3D_MAX_HEIGHT, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE3D_MAX_WIDTH, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE_BASE_ADDRESS_ALIGNMENT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE_MAX_ARRAY_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE_MAX_BUFFER_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE_PITCH_ALIGNMENT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(IMAGE_SUPPORT, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(LOCAL_MEM_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(LOCAL_MEM_TYPE, devlocal_mem_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_CLOCK_FREQUENCY, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_COMPUTE_UNITS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_CONSTANT_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_CONSTANT_BUFFER_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_GLOBAL_VARIABLE_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_MEM_ALLOC_SIZE, uint64_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_NUM_SUB_GROUPS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_ON_DEVICE_EVENTS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_ON_DEVICE_QUEUES, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_PARAMETER_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_PIPE_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_READ_IMAGE_ARGS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(MAX_READ_WRITE_IMAGE_ARGS, uint32_t)
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
__TEUTHID_CLB_DEVICE_INFO_SPEC(PARTITION_MAX_SUB_DEVICES, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PARTITION_TYPE, partition_properties_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PIPE_MAX_ACTIVE_RESERVATIONS, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PIPE_MAX_PACKET_SIZE, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_GLOBAL_ATOMIC_ALIGNMENT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_INTEROP_USER_SYNC, bool)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_LOCAL_ATOMIC_ALIGNMENT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_PLATFORM_ATOMIC_ALIGNMENT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_CHAR, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_SHORT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_INT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_LONG, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_FLOAT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_DOUBLE, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PREFERRED_VECTOR_WIDTH_HALF, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PRINTF_BUFFER_SIZE, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PROFILE, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(PROFILING_TIMER_RESOLUTION, std::size_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(QUEUE_ON_DEVICE_MAX_SIZE, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(QUEUE_ON_DEVICE_PREFERRED_SIZE, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(QUEUE_ON_DEVICE_PROPERTIES,
                               devcommand_queue_properties_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(QUEUE_ON_HOST_PROPERTIES,
                               devcommand_queue_properties_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(REFERENCE_COUNT, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(SINGLE_FP_CONFIG, devfp_config_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(SVM_CAPABILITIES, devsvm_capabilities_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(TYPE, devtype_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(VENDOR, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(VENDOR_ID, uint32_t)
__TEUTHID_CLB_DEVICE_INFO_SPEC(VERSION, std::string)
__TEUTHID_CLB_DEVICE_INFO_SPEC(DRIVER_VERSION, std::string)
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

// specialization of device::preferred_vector_width<>()
template <> uint32_t device::preferred_vector_width<int8_t>() const;
template <> uint32_t device::preferred_vector_width<uint8_t>() const;
template <> uint32_t device::preferred_vector_width<int16_t>() const;
template <> uint32_t device::preferred_vector_width<uint16_t>() const;
template <> uint32_t device::preferred_vector_width<int32_t>() const;
template <> uint32_t device::preferred_vector_width<uint32_t>() const;
template <> uint32_t device::preferred_vector_width<int64_t>() const;
template <> uint32_t device::preferred_vector_width<uint64_t>() const;
template <> uint32_t device::preferred_vector_width<float16_t>() const;
template <> uint32_t device::preferred_vector_width<float32_t>() const;
template <> uint32_t device::preferred_vector_width<float64_t>() const;
#endif // DOXYGEN_SHOULD_SKIP_THIS
} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_DEVICE_HPP