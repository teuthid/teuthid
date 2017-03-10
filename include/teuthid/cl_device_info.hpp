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

#ifndef TEUTHID_CL_DEVICE_INFO_HPP
#define TEUTHID_CL_DEVICE_INFO_HPP

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
typedef cl_device_id CL_device_id_t;
#else
typedef int *CL_device_id_t;
#endif // defined(TEUTHID_WITH_OPENCL)
#endif // DOXYGEN_SHOULD_SKIP_THIS

class CL_device_info;
class CL_platform_info;

typedef std::vector<CL_device_info> CL_devices_t;

enum CL_profile_t { CL_FULL_PROFILE, CL_EMBEDDED_PROFILE, CL_UNKNOWN_PROFILE };

class CL_device_info {
  friend class CL_platform_info;

public:
  CL_device_info()
      : id_(NULL), name_(""), version_(""), driver_version_(""), c_version_(""),
        max_compute_units_(0) {}
  ~CL_device_info() {}
  const CL_device_id_t &id() const { return id_; }
  const std::string &name() const { return name_; }
  const std::string &version() const { return version_; }
  const std::string &driver_version() const { return driver_version_; }
  const std::string &c_version() const { return c_version_; }
  uint32_t max_compute_units() const { return max_compute_units_; }

private:
  CL_device_id_t id_;          // device ID
  std::string name_;           // CL_DEVICE_NAME
  std::string version_;        // CL_DEVICE_VERSION
  std::string driver_version_; // CL_DRIVER_VERSION
  std::string c_version_;      // CL_DEVICE_OPENCL_C_VERSION
  uint32_t max_compute_units_; // CL_DEVICE_MAX_COMPUTE_UNITS
};

} // namespace teuthid

#endif // TEUTHID_CL_DEVICE_INFO_HPP