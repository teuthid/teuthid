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
#include <stdexcept>

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
std::streamsize system::float_precision_ = default_float_precision_;
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

#define __TEUTHID_STRING_FROM_INTEGER(TYPE, CAST_TYPE)                         \
  template <> std::string system::to_string(const TYPE &value) {               \
    return std::to_string(static_cast<CAST_TYPE>(value));                      \
  }

__TEUTHID_STRING_FROM_INTEGER(int8_t, long long);
__TEUTHID_STRING_FROM_INTEGER(int16_t, long long);
__TEUTHID_STRING_FROM_INTEGER(int32_t, long long);
__TEUTHID_STRING_FROM_INTEGER(int64_t, long long);
__TEUTHID_STRING_FROM_INTEGER(uint8_t, unsigned long long);
__TEUTHID_STRING_FROM_INTEGER(uint16_t, unsigned long long);
__TEUTHID_STRING_FROM_INTEGER(uint32_t, unsigned long long);
__TEUTHID_STRING_FROM_INTEGER(uint64_t, unsigned long long);
#undef __TEUTHID_STRING_FROM_INTEGER

#ifdef TEUTHID_HAVE_INT_128
std::string __teuthid_system_uint128_to_string(const uint128_t &value) {
  unsigned __mod;
  uint128_t __div, __value = value;
  std::string __s;
  do {
    __div = __value / 10;
    __mod = __value % 10;
    __s += std::to_string(__mod);
    __value = __div;
  } while (__div > 0);
  std::reverse(__s.begin(), __s.end());
  return std::string(__s);
}

template <> std::string system::to_string(const int128_t &value) {
  if ((value < INT64_MIN) || (value > INT64_MAX)) {
    bool __minus = (value < 0);
    uint128_t __value = (value >= 0) ? value : -value;
    std::string __s = __teuthid_system_uint128_to_string(__value);
    __s = __minus ? ("-" + __s) : __s;
    return std::string(__s);
  } else
    return std::to_string(static_cast<long long>(value));
}

template <> std::string system::to_string(const uint128_t &value) {
  if (value > UINT64_MAX)
    return __teuthid_system_uint128_to_string(value);
  else
    return std::to_string(static_cast<unsigned long long>(value));
}
#endif // TEUTHID_HAVE_INT_128

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
  assert(precision > 0);
  std::lock_guard<std::mutex> __guard(system::mutex_);
  system::float_precision_ = precision;
  system::float_scientific_format_ = scientific;
}

std::streamsize system::format_float_precision(std::streamsize precision) {
  assert(precision > 0);
  std::lock_guard<std::mutex> __guard(system::mutex_);
  std::streamsize __prev = system::float_precision_;
  system::float_precision_ = precision;
  return __prev;
}

bool system::format_float_scientific(bool scientific) {
  std::lock_guard<std::mutex> __guard(system::mutex_);
  bool __prev = system::float_scientific_format_;
  system::float_scientific_format_ = scientific;
  return __prev;
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
bool &system::from_string(const std::string &str_value, bool &value) {
  std::string __s = __teuthid_system_validate_string(str_value);
  if (!__s.empty())
    if ((__s == "false") || (__s == "true")) {
      value = (__s == "true");
      return value;
    }
  throw std::invalid_argument("empty or invalid string");
}

#define __TEUTHID_SIGNED_INTEGER_FROM_STRING(TYPE)                             \
  template <>                                                                  \
  TYPE &system::from_string(const std::string &str_value, TYPE &value) {       \
    std::string __s = __teuthid_system_validate_string(str_value);             \
    if (!__s.empty()) {                                                        \
      long long __long = std::stoll(__s);                                      \
      TYPE __val = __long;                                                     \
      if (__long != __val)                                                     \
        throw std::out_of_range("system::from_string()");                      \
      value = __val;                                                           \
      return value;                                                            \
    }                                                                          \
    throw std::invalid_argument("empty or invalid string");                    \
  }

__TEUTHID_SIGNED_INTEGER_FROM_STRING(int8_t);
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int16_t);
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int32_t);
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int64_t);
#undef __TEUTHID_SIGNED_INTEGER_FROM_STRING

#define __TEUTHID_UNSIGNED_INTEGER_FROM_STRING(TYPE)                           \
  template <>                                                                  \
  TYPE &system::from_string(const std::string &str_value, TYPE &value) {       \
    std::string __s = __teuthid_system_validate_string(str_value);             \
    if (!__s.empty()) {                                                        \
      unsigned long long __long = std::stoull(__s);                            \
      TYPE __val = __long;                                                     \
      if (__long != __val)                                                     \
        throw std::out_of_range("system::from_string()");                      \
      value = __val;                                                           \
      return value;                                                            \
    }                                                                          \
    throw std::invalid_argument("empty or invalid string");                    \
  }

__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint8_t);
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint16_t);
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint32_t);
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint64_t);
#undef __TEUTHID_UNSIGNED_INTEGER_FROM_STRING

#define __TEUTHID_FLOAT_FROM_STRING(TYPE, FUN)                                 \
  template <>                                                                  \
  TYPE &system::from_string(const std::string &str_value, TYPE &value) {       \
    std::string __s = __teuthid_system_validate_string(str_value);             \
    if (!__s.empty()) {                                                        \
      value = std::FUN(__s);                                                   \
      return value;                                                            \
    }                                                                          \
    throw std::invalid_argument("empty or invalid string");                    \
  }

__TEUTHID_FLOAT_FROM_STRING(float, stof);
__TEUTHID_FLOAT_FROM_STRING(double, stod);
__TEUTHID_FLOAT_FROM_STRING(long double, stold);
#undef __TEUTHID_FLOAT_FROM_STRING

template <>
mpfr_t &system::from_string(const std::string &str_value, mpfr_t &value) {
  std::string __s = __teuthid_system_validate_string(str_value);
  if (!__s.empty())
    if (mpfr_set_str(value, __s.c_str(), 10,
                     mpfr_get_default_rounding_mode()) == 0)
      return value;
  throw std::invalid_argument("empty or invalid string");
}
#endif // DOXYGEN_SHOULD_SKIP_THIS
