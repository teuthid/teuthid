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

enum devparam_t {
  ADDRESS_BITS = CL_DEVICE_ADDRESS_BITS,
  AVAILABLE = CL_DEVICE_AVAILABLE,
  BUILT_IN_KERNELS = CL_DEVICE_BUILT_IN_KERNELS,
  COMPILER_AVAILABLE = CL_DEVICE_COMPILER_AVAILABLE,
  DOUBLE_FP_CONFIG = CL_DEVICE_DOUBLE_FP_CONFIG,
  ENDIAN_LITTLE = CL_DEVICE_ENDIAN_LITTLE
};

enum devfp_config_t {
  DENORM = CL_FP_DENORM,
  INF_NAN = CL_FP_INF_NAN,
  ROUND_TO_NEAREST = CL_FP_ROUND_TO_NEAREST,
  ROUND_TO_ZERO = CL_FP_ROUND_TO_ZERO,
  ROUND_TO_INF = CL_FP_ROUND_TO_INF,
  FMA = CL_FP_FMA,
  SOFT_FLOAT = CL_FP_SOFT_FLOAT,
  CORRECTLY_ROUNDED_DIVIDE_SQRT = CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT
};

enum devtype_t {
  CPU = CL_DEVICE_TYPE_CPU,
  GPU = CL_DEVICE_TYPE_GPU,
  ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
  CUSTOM = CL_DEVICE_TYPE_CUSTOM
};

template <devparam_t> struct device_param { typedef std::string value_type; };

class device;
class platform;

typedef cl_device_id device_id_t;
typedef cl_platform_id platform_id_t;
typedef std::vector<std::string> extensions_t;
typedef std::vector<std::string> built_in_kernels_t;
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
  const std::string &name() const noexcept { return name_; }
  const std::string &version() const noexcept { return version_; }
  const std::string &driver_version() const noexcept { return driver_version_; }
  const std::string &c_version() const noexcept { return c_version_; }
  uint32_t max_compute_units() const noexcept { return max_compute_units_; }
  const extensions_t &extensions() const noexcept { return extensions_; }
  bool have_extension(const std::string &ext_name) const;
  uint32_t address_bits() const;
  bool is_available() const;
  built_in_kernels_t built_in_kernels() const;
  bool is_compiler_available() const;
  devfp_config_t double_fp_config() const;
  bool have_double_precision() const;
  uint64_t global_memory_size() const noexcept { return global_memory_size_; }
  uint64_t local_memory_size() const noexcept { return local_memory_size_; }

  bool operator==(const device &other) const { return id_ == other.id_; }
  bool operator!=(const device &other) const { return id_ != other.id_; }

  static const platform &get_platform(device_id_t device_id);

private:
  device() {}

  device_id_t id_;              // device ID
  platform_id_t platform_id_;   // platform ID
  profile_t profile_;           // CL_DEVICE_PROFILE
  devtype_t devtype_;           // CL_DEVICE_TYPE
  std::string name_;            // CL_DEVICE_NAME
  std::string version_;         // CL_DEVICE_VERSION
  std::string driver_version_;  // CL_DRIVER_VERSION
  std::string c_version_;       // CL_DEVICE_OPENCL_C_VERSION
  uint32_t max_compute_units_;  // CL_DEVICE_MAX_COMPUTE_UNITS
  extensions_t extensions_;     // CL_DEVICE_EXTENSIONS
  uint64_t global_memory_size_; // CL_DEVICE_GLOBAL_MEM_SIZE
  uint64_t local_memory_size_;  // CL_DEVICE_LOCAL_MEM_SIZE
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// specialization of device_param<>::value_type info()
template <> struct device_param<devparam_t::ADDRESS_BITS> {
  typedef uint32_t value_type;
};
template <>
device_param<devparam_t::ADDRESS_BITS>::value_type
device::info<devparam_t::ADDRESS_BITS>() const;

template <> struct device_param<devparam_t::AVAILABLE> {
  typedef bool value_type;
};
template <>
device_param<devparam_t::AVAILABLE>::value_type
device::info<devparam_t::AVAILABLE>() const;

template <>
device_param<devparam_t::BUILT_IN_KERNELS>::value_type
device::info<devparam_t::BUILT_IN_KERNELS>() const;

template <> struct device_param<devparam_t::COMPILER_AVAILABLE> {
  typedef bool value_type;
};
template <>
device_param<devparam_t::COMPILER_AVAILABLE>::value_type
device::info<devparam_t::COMPILER_AVAILABLE>() const;

template <> struct device_param<devparam_t::DOUBLE_FP_CONFIG> {
  typedef devfp_config_t value_type;
};
template <>
device_param<devparam_t::DOUBLE_FP_CONFIG>::value_type
device::info<devparam_t::DOUBLE_FP_CONFIG>() const;

template <> struct device_param<devparam_t::ENDIAN_LITTLE> {
  typedef bool value_type;
};
template <>
device_param<devparam_t::ENDIAN_LITTLE>::value_type
device::info<devparam_t::ENDIAN_LITTLE>() const;

#endif // DOXYGEN_SHOULD_SKIP_THIS
} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_DEVICE_HPP