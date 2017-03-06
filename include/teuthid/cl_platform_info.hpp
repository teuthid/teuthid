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

#ifndef TEUTHID_CL_INFO_HPP

#include <mutex>
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

namespace cl {
class platform_info;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(TEUTHID_WITH_OPENCL)
typedef cl_platform_id opencl_platform_id_t;
#else
typedef int *opencl_platform_id_t;
#endif // defined(TEUTHID_WITH_OPENCL)
#endif // DOXYGEN_SHOULD_SKIP_THIS

typedef std::vector<cl::platform_info> opencl_platforms_t;

namespace cl {

class platform_info {
public:
  enum opencl_profile_t { FULL_PROFILE, EMBEDDED_PROFILE, UNKNOWN_PROFILE };

  platform_info()
      : id_(NULL), profile_(UNKNOWN_PROFILE), version_(""), major_version_(0),
        minor_version_(0), spec_version_(""), name_(""), vendor_(""),
        host_timer_resolution_(0), icd_suffix_khr_(""), extensions_("") {}
  ~platform_info() {}
  opencl_platform_id_t id() const { return id_; }
  opencl_profile_t profile() const { return profile_; }
  bool is_full_profile() const { return (profile_ == FULL_PROFILE); }
  bool is_embedded_profile() const { return (profile_ == EMBEDDED_PROFILE); }
  const std::string &version() const { return version_; }
  int major_version() const { return major_version_; }
  int minor_version() const { return minor_version_; }
  const std::string &spec_version() const { return spec_version_; }
  const std::string &name() const { return name_; }
  const std::string &vendor() const { return vendor_; }
  const std::string &extensions() const { return extensions_; }
  uint64_t host_timer_resolution() const { return host_timer_resolution_; }
  const std::string &icd_suffix_khr() const { return icd_suffix_khr_; }

  static const opencl_platforms_t &platforms(bool force_detection = false);

private:
  opencl_platform_id_t id_;  // platform ID
  opencl_profile_t profile_; // CL_PLATFORM_PROFILE
  std::string version_;      // CL_PLATFORM_VERSION
  int major_version_;
  int minor_version_;
  std::string spec_version_;
  std::string name_;               // CL_PLATFORM_NAME
  std::string vendor_;             // CL_PLATFORM_VENDOR
  std::string extensions_;         // CL_PLATFORM_EXTENSIONS
  uint64_t host_timer_resolution_; // CL_PLATFORM_HOST_TIMER_RESOLUTION
  std::string icd_suffix_khr_;     // CL_PLATFORM_ICD_SUFFIX_KHR

  static std::mutex mutex_;
  static bool platforms_detected_;
  static opencl_platforms_t platforms_;
  static void detect_platforms_();
};

} // namespace cl
} // namespace teuthid

#endif // TEUTHID_CL_INFO_HPP