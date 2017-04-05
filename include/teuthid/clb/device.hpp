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

class device;
class platform;

typedef cl_device_id device_id_t;
typedef cl_platform_id platform_id_t;
typedef std::vector<std::string> extensions_t;
typedef std::vector<device> devices_t;
typedef std::vector<platform> platforms_t;

enum profile_t { FULL_PROFILE, EMBEDDED_PROFILE, UNKNOWN_PROFILE };
enum devtype_t { DEVICE_CPU, DEVICE_GPU, DEVICE_ACCELERATOR, DEVICE_UNKNOWN };

class device {
  friend class platform;

public:
  explicit device(device_id_t device_id);
  device(const device &) = default;
  device(device &&) = default;
  virtual ~device() {}
  device &operator=(const device &) = default;
  device &operator=(device &&) = default;
  const device_id_t &id() const noexcept { return id_; }
  const platform &get_platform() const;
  const profile_t &profile() const noexcept { return profile_; }
  bool is_full_profile() const noexcept { return (profile_ == FULL_PROFILE); }
  bool is_embedded_profile() const noexcept {
    return (profile_ == EMBEDDED_PROFILE);
  }
  const devtype_t &devtype() const noexcept { return devtype_; }
  bool is_devtype_cpu() const noexcept { return (devtype_ == DEVICE_CPU); }
  bool is_devtype_gpu() const noexcept { return (devtype_ == DEVICE_GPU); }
  bool is_devtype_accelerator() const noexcept {
    return (devtype_ == DEVICE_ACCELERATOR);
  }
  const std::string &name() const noexcept { return name_; }
  const std::string &version() const noexcept { return version_; }
  const std::string &driver_version() const noexcept { return driver_version_; }
  const std::string &c_version() const noexcept { return c_version_; }
  uint32_t max_compute_units() const noexcept { return max_compute_units_; }
  const extensions_t &extensions() const noexcept { return extensions_; }
  bool have_extension(const std::string &ext_name) const;
  uint8_t address_bits() const noexcept { return address_bits_; }
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
  uint8_t address_bits_;        // CL_DEVICE_ADDRESS_BITS
  uint64_t global_memory_size_; // CL_DEVICE_GLOBAL_MEM_SIZE
  uint64_t local_memory_size_;  // CL_DEVICE_LOCAL_MEM_SIZE
};

} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_DEVICE_HPP