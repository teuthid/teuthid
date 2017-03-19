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

#ifndef TEUTHID_CLB_PLATFORM_HPP
#define TEUTHID_CLB_PLATFORM_HPP

#include <mutex>
#include <string>
#include <vector>

#include <teuthid/clb/device.hpp>

namespace teuthid {
namespace clb {

class platform;

typedef cl_platform_id platform_id_t;
typedef std::vector<platform> platforms_t;

class platform {
public:
  explicit platform(platform_id_t id);
  platform(const platform &) = default;
  platform(platform &&) = default;
  virtual ~platform() {}
  platform &operator=(const platform &) = default;
  platform &operator=(platform &&) = default;
  const platform_id_t &id() const noexcept { return id_; }
  const profile_t &profile() const noexcept { return profile_; }
  bool is_full_profile() const noexcept { return (profile_ == FULL_PROFILE); }
  bool is_embedded_profile() const noexcept {
    return (profile_ == EMBEDDED_PROFILE);
  }
  const std::string &version() const noexcept { return version_; }
  int major_version() const noexcept { return major_version_; }
  int minor_version() const noexcept { return minor_version_; }
  const std::string &spec_version() const noexcept { return spec_version_; }
  bool is_required_version(int major, int minor) const noexcept;
  const std::string &name() const noexcept { return name_; }
  const std::string &vendor() const noexcept { return vendor_; }
  const extensions_t &extensions() const noexcept { return extensions_; }
  bool have_extension(const std::string &ext_name) const;
  const std::string &icd_suffix_khr() const noexcept { return icd_suffix_khr_; }
  const devices_t &devices() const noexcept { return devices_; }
  std::size_t device_count() const noexcept { return devices_.size(); }
  bool unload_compiler();

  static const platforms_t &platforms();
  static std::size_t platform_count() { return platforms().size(); }

private:
  platform() {}

  platform_id_t id_;    // platform ID
  profile_t profile_;   // CL_PLATFORM_PROFILE
  std::string version_; // CL_PLATFORM_VERSION
  int major_version_;
  int minor_version_;
  std::string spec_version_;
  std::string name_;           // CL_PLATFORM_NAME
  std::string vendor_;         // CL_PLATFORM_VENDOR
  extensions_t extensions_;    // CL_PLATFORM_EXTENSIONS
  std::string icd_suffix_khr_; // CL_PLATFORM_ICD_SUFFIX_KHR
  devices_t devices_;          // devices of this platform

  static std::mutex mutex_;
  static bool platforms_detected_;
  static platforms_t platforms_;
  static void detect_platforms_();
  static void detect_devices_(platform &);
};

} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_PLATFORM_HPP