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

#include <cassert>

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
  sprintf(__precision, "%%.%ldRe", system::float_precision());
  mpfr_sprintf(__str, __precision, value);
  return std::string(__str);
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

std::streamsize system::float_precision(std::streamsize precision) {
  assert(precision >= 0);
  std::lock_guard<std::mutex> __guard(system::mutex_);
  system::float_precision_ = precision;
  return system::float_precision_;
}