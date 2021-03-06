/*
  This file is part of the Teuthid project.
  Copyright (c) 2016-2017 Mariusz Przygodzki (mariusz.przygodzki@gmail.com)

  The Teuthid is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  The Teuthid is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
  for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the Teuthid; see the file LICENSE.LGPLv3.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#ifndef TEUTHID_CLB_PLATFORM_HPP
#define TEUTHID_CLB_PLATFORM_HPP

#include <mutex>
#include <string>
#include <vector>

#include <teuthid/clb/device.hpp>

namespace teuthid {
namespace clb {

enum class platparam_t : uint64_t { // cl_platform_info
  PROFILE = CL_PLATFORM_PROFILE,
  VERSION = CL_PLATFORM_VERSION,
  NAME = CL_PLATFORM_NAME,
  VENDOR = CL_PLATFORM_VENDOR,
  EXTENSIONS = CL_PLATFORM_EXTENSIONS,
  HOST_TIMER_RESOLUTION = CL_PLATFORM_HOST_TIMER_RESOLUTION,
  ICD_SUFFIX_KHR = CL_PLATFORM_ICD_SUFFIX_KHR
};
using platprofile_t = devprofile_t;

template <platparam_t> struct platform_param { typedef void value_type; };

class platform;
typedef std::vector<platform> platforms_t;

class platform {
public:
  platform(const platform &) = default;
  platform(platform &&) = default;
  virtual ~platform() {}
  platform &operator=(const platform &) = default;
  platform &operator=(platform &&) = default;

  template <platparam_t value>
  typename platform_param<value>::value_type info() const;

  const platform_id_t &id() const noexcept { return id_; }
  platprofile_t profile() const;
  bool is_full_profile() const { return (profile() == platprofile_t::FULL); }
  bool is_embedded_profile() const {
    return (profile() == platprofile_t::EMBEDDED);
  }
  std::string version() const;
  bool check_version(int major, int minor) const;
  std::string name() const;
  std::string vendor() const;
  extensions_t extensions() const;
  bool has_extension(const std::string &ext_name) const;
  uint64_t host_timer_resolution() const;
  std::string icd_suffix_khr() const;
  const devices_t &devices() const noexcept { return devices_; }
  std::size_t device_count() const noexcept { return devices_.size(); }
  bool unload_compiler();

  bool operator==(const platform &other) const { return id_ == other.id_; }
  bool operator!=(const platform &other) const { return id_ != other.id_; }

  static const platform &find_by_id(platform_id_t platform_id);
  static const platforms_t &get_all();
  static const platform &get_default();
  static const platform &set_default(const platform &plat);
  static std::size_t count() { return platform::get_all().size(); }

private:
  platform() {}
  explicit platform(platform_id_t id) : id_(id) {}
  platform_id_t id_;             // platform ID
  devices_t devices_;            // devices of this platform
  static platforms_t platforms_; // all available platforms
  static std::mutex detect_mutex_;
  static void detect_platforms_();
  static void detect_devices_(platform &);
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// specialization of platform::info<>()
#define __TEUTHID_CLB_PLATFORM_INFO_SPEC(PARAM, VALUE_TYPE)                    \
  template <> struct platform_param<platparam_t::PARAM> {                      \
    typedef VALUE_TYPE value_type;                                             \
  };                                                                           \
  template <>                                                                  \
  platform_param<platparam_t::PARAM>::value_type                               \
  platform::info<platparam_t::PARAM>() const;

__TEUTHID_CLB_PLATFORM_INFO_SPEC(PROFILE, std::string)
__TEUTHID_CLB_PLATFORM_INFO_SPEC(VERSION, std::string)
__TEUTHID_CLB_PLATFORM_INFO_SPEC(NAME, std::string)
__TEUTHID_CLB_PLATFORM_INFO_SPEC(VENDOR, std::string)
__TEUTHID_CLB_PLATFORM_INFO_SPEC(EXTENSIONS, std::string)
__TEUTHID_CLB_PLATFORM_INFO_SPEC(HOST_TIMER_RESOLUTION, uint64_t)
__TEUTHID_CLB_PLATFORM_INFO_SPEC(ICD_SUFFIX_KHR, std::string)
#undef __TEUTHID_CLB_PLATFORM_INFO_SPEC
#endif // DOXYGEN_SHOULD_SKIP_THIS
} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_PLATFORM_HPP