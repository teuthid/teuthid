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

namespace teuthid {

namespace cl {
class platform_info;
}

typedef std::vector<cl::platform_info> opencl_platforms_t;

namespace cl {

class platform_info {
public:
  enum opencl_profile_t { FULL_PROFILE, EMBEDDED_PROFILE, UNKNOWN_PROFILE };

  platform_info()
      : profile_(UNKNOWN_PROFILE), major_version_(0), minor_version_(0),
        spec_version_(std::string("")), version_(std::string("")),
        name_(std::string("")), vendor_(std::string("")),
        host_timer_resolution_(0), icd_suffix_khr_(std::string("")) {
    extensions_ = std::vector<std::string>();
  }
  ~platform_info() {}
  opencl_profile_t profile() const { return profile_; }
  int major_version() const { return major_version_; }
  int minor_version() const { return minor_version_; }
  const std::string &spec_version() const { return spec_version_; }
  const std::string &version() const { return version_; }
  const std::string &name() const { return name_; }
  const std::string &vendor() const { return vendor_; }
  const std::vector<std::string> &extensions() const { return extensions_; }
  uint64_t host_timer_resolution() const { return host_timer_resolution_; }
  const std::string &icd_suffix_khr() const { return icd_suffix_khr_; }

  static const opencl_platforms_t &platforms(bool force_detection = false);

private:
  opencl_profile_t profile_; // CL_PLATFORM_PROFILE (char[])
  int major_version_;        // CL_PLATFORM_VERSION (char[])
  int minor_version_;        // <major.minor><space><platform-specific>
  std::string spec_version_;
  std::string version_;
  std::string name_;                    // CL_PLATFORM_NAME (char[])
  std::string vendor_;                  // CL_PLATFORM_VENDOR (char[])
  std::vector<std::string> extensions_; // CL_PLATFORM_EXTENSIONS (char[])
  uint64_t
      host_timer_resolution_;  // CL_PLATFORM_HOST_TIMER_RESOLUTION (cl_ulong)
  std::string icd_suffix_khr_; // CL_PLATFORM_ICD_SUFFIX_KHR (char[])

  static std::mutex mutex_;
  static bool platforms_detected_;
  static opencl_platforms_t platforms_;
  static void detect_platforms_();
};

} // namespace cl
} // namespace teuthid

#endif // TEUTHID_CL_INFO_HPP