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

#ifndef TEUTHID_COMPUTE_DEVICE_HPP
#define TEUTHID_COMPUTE_DEVICE_HPP

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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(TEUTHID_WITH_OPENCL)
typedef cl_device_id compute_device_id_t;
#else
typedef int *compute_device_id_t;
#endif // defined(TEUTHID_WITH_OPENCL)
#endif // DOXYGEN_SHOULD_SKIP_THIS

class compute_device;
class compute_platform;
typedef std::vector<compute_device> compute_devices_t;

enum compute_profile_t {
  COMPUTE_FULL_PROFILE,
  COMPUTE_EMBEDDED_PROFILE,
  COMPUTE_UNKNOWN_PROFILE
};

enum compute_devtype_t {
  COMPUTE_DEVICE_CPU,
  COMPUTE_DEVICE_GPU,
  COMPUTE_DEVICE_ACCELERATOR,
  COMPUTE_DEVICE_UNKNOWN
};

class compute_device {
  friend class compute_platform;

public:
  compute_device()
      : id_(NULL), profile_(COMPUTE_UNKNOWN_PROFILE),
        devtype_(COMPUTE_DEVICE_UNKNOWN), max_compute_units_(0) {}
  compute_device(const compute_device &) = default;
  ~compute_device() {}
  compute_device &operator=(const compute_device &) = default;
  const compute_device_id_t &id() const { return id_; }
  const compute_profile_t &profile() const { return profile_; }
  bool is_full_profile() const { return (profile_ == COMPUTE_FULL_PROFILE); }
  bool is_embedded_profile() const {
    return (profile_ == COMPUTE_EMBEDDED_PROFILE);
  }
  const compute_devtype_t &devtype() { return devtype_; }
  bool is_devtype_cpu() { return (devtype_ == COMPUTE_DEVICE_CPU); }
  bool is_devtype_gpu() { return (devtype_ == COMPUTE_DEVICE_GPU); }
  bool is_devtype_accelerator() {
    return (devtype_ == COMPUTE_DEVICE_ACCELERATOR);
  }
  const std::string &name() const { return name_; }
  const std::string &version() const { return version_; }
  const std::string &driver_version() const { return driver_version_; }
  const std::string &c_version() const { return c_version_; }
  uint32_t max_compute_units() const { return max_compute_units_; }
  const std::string &extensions() const { return extensions_; }

private:
  compute_device_id_t id_;     // device ID
  compute_profile_t profile_;  // CL_DEVICE_PROFILE
  compute_devtype_t devtype_;  // CL_DEVICE_TYPE
  std::string name_;           // CL_DEVICE_NAME
  std::string version_;        // CL_DEVICE_VERSION
  std::string driver_version_; // CL_DRIVER_VERSION
  std::string c_version_;      // CL_DEVICE_OPENCL_C_VERSION
  uint32_t max_compute_units_; // CL_DEVICE_MAX_COMPUTE_UNITS
  std::string extensions_;     // CL_DEVICE_EXTENSIONS
};

} // namespace teuthid

#endif // TEUTHID_COMPUTE_DEVICE_HPP