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

#ifndef TEUTHID_COMPUTE_PLATFORM_HPP
#define TEUTHID_COMPUTE_PLATFORM_HPP

#include <mutex>
#include <string>
#include <vector>

#include <teuthid/compute_device.hpp>

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
typedef cl_platform_id compute_platform_id_t;
#else
typedef int *compute_platform_id_t;
#endif // defined(TEUTHID_WITH_OPENCL)
#endif // DOXYGEN_SHOULD_SKIP_THIS

class compute_platform;
typedef std::vector<compute_platform> compute_platforms_t;

class compute_platform {
public:
  compute_platform() noexcept : id_(NULL),
                                profile_(COMPUTE_UNKNOWN_PROFILE),
                                major_version_(0),
                                minor_version_(0) {}
  compute_platform(const compute_platform &) = default;
  virtual ~compute_platform() {}
  compute_platform &operator=(const compute_platform &) = default;
  const compute_platform_id_t &id() const noexcept { return id_; }
  const compute_profile_t &profile() const noexcept { return profile_; }
  bool is_full_profile() const noexcept {
    return (profile_ == COMPUTE_FULL_PROFILE);
  }
  bool is_embedded_profile() const noexcept {
    return (profile_ == COMPUTE_EMBEDDED_PROFILE);
  }
  const std::string &version() const noexcept { return version_; }
  int major_version() const noexcept { return major_version_; }
  int minor_version() const noexcept { return minor_version_; }
  const std::string &spec_version() const noexcept { return spec_version_; }
  bool is_required_version(int major, int minor) const noexcept;
  const std::string &name() const noexcept { return name_; }
  const std::string &vendor() const noexcept { return vendor_; }
  const std::string &extensions() const noexcept { return extensions_; }
  const std::string &icd_suffix_khr() const noexcept { return icd_suffix_khr_; }
  const compute_devices_t &devices() const noexcept { return devices_; }
  std::size_t num_devices() const noexcept { return devices_.size(); }

  static const compute_platforms_t &platforms();

private:
  compute_platform_id_t id_;  // platform ID
  compute_profile_t profile_; // CL_PLATFORM_PROFILE
  std::string version_;       // CL_PLATFORM_VERSION
  int major_version_;
  int minor_version_;
  std::string spec_version_;
  std::string name_;           // CL_PLATFORM_NAME
  std::string vendor_;         // CL_PLATFORM_VENDOR
  std::string extensions_;     // CL_PLATFORM_EXTENSIONS
  std::string icd_suffix_khr_; // CL_PLATFORM_ICD_SUFFIX_KHR
  compute_devices_t devices_;  // devices of this platform

  static std::mutex mutex_;
  static bool platforms_detected_;
  static compute_platforms_t platforms_;
  static void detect_platforms_();
  static void detect_devices_(compute_platform &);
};

} // namespace teuthid

#endif // TEUTHID_COMPUTE_PLATFORM_HPP