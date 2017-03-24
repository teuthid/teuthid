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

#include <algorithm>
#include <cassert>
#include <cctype>

#include <teuthid/system.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#include <teuthid/clb/error.hpp>
#include <teuthid/clb/platform.hpp>
#endif

using namespace teuthid;

#if defined(TEUTHID_WITH_OPENCL)
thread_local bool system::use_clb_ = system::have_clb();
#else
thread_local bool system::use_clb_ = false;
#endif

std::string system::version_ = std::string(TEUTHID_VERSION);
std::mutex system::mutex_;
std::streamsize system::float_precision_ = 10;
bool system::float_scientific_format_ = false;

bool system::is_required_version(uint8_t major, uint8_t minor) noexcept {
  uint32_t __required = major * 1000 + minor;
  uint32_t __actual = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(__required > __actual));
}

bool system::have_clb() {
#if defined(TEUTHID_WITH_OPENCL)
  try {
    for (auto __platform : clb::platform::platforms())
      if (__platform.device_count() > 0)
        return true;
  } catch (const clb::error &) {
    // some problems with the compute kernel - it wil be disabled
  }
#endif // TEUTHID_WITH_OPENCL
  return false;
}

bool system::use_clb(bool enabled) {
  if (system::have_clb())
    system::use_clb_ = enabled;
  return system::use_clb_;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
std::string system::to_string(const std::vector<std::string> &value) {
  std::string __str;
  for (std::size_t __i = 0; __i < value.size(); __i++) {
    __str += value[__i];
    if ((__i + 1) < value.size())
      __str += " ";
  }
  return std::string(__str);
}

template <> std::string system::to_string(const mpfr_t &value) {
  char __str[256], __precision[64];
  std::string __format =
      (system::float_scientific_format_ ? "%%.%ldRe" : "%%.%ldRf");
  sprintf(__precision, __format.c_str(), system::float_precision_);
  mpfr_sprintf(__str, __precision, value);
  return std::string(__str);
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

void system::format_float_output(std::streamsize precision, bool scientific) {
  assert(precision >= 0);
  std::lock_guard<std::mutex> __guard(system::mutex_);
  system::float_precision_ = precision;
  system::float_scientific_format_ = scientific;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
std::string __teuthid_system_validate_string(const std::string &str) {
  std::string __s = str;
  if (!__s.empty()) {
    std::transform(__s.begin(), __s.end(), __s.begin(), ::tolower);
    __s.erase(std::remove_if(__s.begin(), __s.end(), ::isspace), __s.end());
  }
  return std::string(__s);
}

template <>
bool system::from_string(const std::string &str_value, bool &value) {
  std::string __s = __teuthid_system_validate_string(str_value);
  if (!__s.empty())
    if ((__s == "false") || (__s == "true")) {
      value = (__s == "true");
      return true;
    }
  return false;
}
#endif // DOXYGEN_SHOULD_SKIP_THIS