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
#include <sstream>
#include <stdexcept>

#include <teuthid/system.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#include <teuthid/clb/error.hpp>
#include <teuthid/clb/platform.hpp>
#endif

using namespace teuthid;

#if defined(TEUTHID_WITH_OPENCL)
std::atomic_bool system::clb_(system::has_cl_backend());
#else
std::atomic_bool system::clb_(false);
#endif

std::string system::version_ = std::string(TEUTHID_VERSION);
std::atomic<std::streamsize>
    system::format_float_precision_(system::default_format_float_precision_);
std::atomic_bool system::format_float_scientific_(false);

bool system::check_version(uint8_t major, uint8_t minor) noexcept {
  return (TEUTHID_MAJOR_VERSION > major ||
          (TEUTHID_MAJOR_VERSION == major && TEUTHID_MINOR_VERSION >= minor));
}

bool system::has_cl_backend() {
#if defined(TEUTHID_WITH_OPENCL)
  try {
    for (auto __platform : clb::platform::get_all())
      if (__platform.device_count() > 0)
        return true;
  } catch (const clb::error &) {
    // some problems with the compute kernel - it wil be disabled
  }
#endif // TEUTHID_WITH_OPENCL
  return false;
}

bool system::uses_cl_backend(bool enabled) {
  if (system::has_cl_backend())
    system::clb_.store(enabled);
  return system::clb_.load();
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifdef TEUTHID_HAVE_INT_128
std::string system::uint128_to_string_(uint128_t x) {
  unsigned __mod;
  uint128_t __div, __value = x;
  std::string __s;
  do {
    __div = __value / 10;
    __mod = __value % 10;
    __s += std::to_string(__mod);
    __value = __div;
  } while (__div > 0);
  std::reverse(__s.begin(), __s.end());
  return __s;
}

template <> std::string system::to_string(const int128_t &x) {
  if ((x < INT64_MIN) || (x > INT64_MAX)) {
    bool __minus = (x < 0);
    uint128_t __value = (x >= 0) ? x : -x;
    std::string __s = system::uint128_to_string_(__value);
    __s = __minus ? ("-" + __s) : __s;
    return __s;
  } else
    return std::to_string(static_cast<long long>(x));
}

template <> std::string system::to_string(const uint128_t &x) {
  if (x > UINT64_MAX)
    return system::uint128_to_string_(x);
  else
    return std::to_string(static_cast<unsigned long long>(x));
}
#endif // TEUTHID_HAVE_INT_128

#define __TEUTHID_STRING_FROM_FLOAT(TYPE)                                      \
  template <> std::string system::to_string(const TYPE &x) {                   \
    std::ostringstream __os;                                                   \
    __os.precision(system::format_float_precision_.load());                    \
    __os << (system::format_float_scientific_.load() ? std::scientific         \
                                                     : std::fixed);            \
    __os << x;                                                                 \
    return __os.str();                                                         \
  }
__TEUTHID_STRING_FROM_FLOAT(float);
__TEUTHID_STRING_FROM_FLOAT(double);
__TEUTHID_STRING_FROM_FLOAT(long double);
#undef __TEUTHID_STRING_FROM_FLOAT

template <> std::string system::to_string(const mpfr_t &x) {
  char __str[512], __precision[64];
  std::string __format =
      (system::format_float_scientific_.load() ? "%%.%ldRe" : "%%.%ldRf");
  sprintf(__precision, __format.c_str(),
          system::format_float_precision_.load());
  mpfr_sprintf(__str, __precision, x);
  return std::string(__str);
}

template <> std::string system::to_string(const floatmp_base &x) {
  return system::to_string(x.c_mpfr());
}

template <> std::string system::to_string(const std::vector<std::string> &v) {
  std::string __str;
  for (std::size_t __i = 0; __i < v.size(); __i++) {
    __str += v[__i];
    if ((__i + 1) < v.size())
      __str += " ";
  }
  return __str;
}

#endif // DOXYGEN_SHOULD_SKIP_THIS

std::size_t system::split_string(const std::string &s,
                                 std::vector<std::string> &v, char delim) {
  v.clear();
  if (!s.empty()) {
    std::size_t __start = 0, __end = 0;
    std::string __sub;
    while ((__end = s.find(delim, __start)) != std::string::npos) {
      if (__end != __start) {
        __sub = s.substr(__start, __end - __start);
        if (!__sub.empty())
          v.push_back(__sub);
      }
      __start = __end + 1;
    }
    if (__end != __start) {
      __sub = s.substr(__start);
      if (!__sub.empty())
        v.push_back(__sub);
    }
    v.shrink_to_fit();
  }
  return v.size();
}

void system::format_float_output(std::streamsize precision, bool scientific) {
  assert(precision > 0);
  system::format_float_precision_.store(precision);
  system::format_float_scientific_.store(scientific);
}

std::streamsize system::format_float_precision(std::streamsize precision) {
  assert(precision > 0);
  return format_float_precision_.exchange(precision);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

std::string system::validate_string_(const std::string &s) {
  std::string __s = s;
  if (!__s.empty()) {
    std::transform(__s.begin(), __s.end(), __s.begin(), ::tolower);
    __s.erase(std::remove_if(__s.begin(), __s.end(), ::isspace), __s.end());
  }
  return __s;
}

template <> bool &system::from_string(const std::string &s, bool &x) {
  std::string __s = system::validate_string_(s);
  if (!__s.empty())
    if ((__s == "false") || (__s == "true") || (__s == "1") || (__s == "0")) {
      x = ((__s == "true") || (__s == "1"));
      return x;
    }
  throw std::invalid_argument("empty or invalid string");
}

#define __TEUTHID_SIGNED_INTEGER_FROM_STRING(TYPE)                             \
  template <> TYPE &system::from_string(const std::string &s, TYPE &x) {       \
    std::string __s = system::validate_string_(s);                             \
    if (!__s.empty()) {                                                        \
      long long __long = std::stoll(__s);                                      \
      TYPE __val = __long;                                                     \
      if (__long != __val)                                                     \
        throw std::out_of_range("system::from_string()");                      \
      x = __val;                                                               \
      return x;                                                                \
    }                                                                          \
    throw std::invalid_argument("empty or invalid string");                    \
  }
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int8_t);
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int16_t);
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int32_t);
__TEUTHID_SIGNED_INTEGER_FROM_STRING(int64_t);
#undef __TEUTHID_SIGNED_INTEGER_FROM_STRING

#define __TEUTHID_UNSIGNED_INTEGER_FROM_STRING(TYPE)                           \
  template <> TYPE &system::from_string(const std::string &s, TYPE &x) {       \
    std::string __s = system::validate_string_(s);                             \
    if (!__s.empty()) {                                                        \
      unsigned long long __long = std::stoull(__s);                            \
      TYPE __val = __long;                                                     \
      if (__long != __val)                                                     \
        throw std::out_of_range("system::from_string()");                      \
      x = __val;                                                               \
      return x;                                                                \
    }                                                                          \
    throw std::invalid_argument("empty or invalid string");                    \
  }
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint8_t);
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint16_t);
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint32_t);
__TEUTHID_UNSIGNED_INTEGER_FROM_STRING(uint64_t);
#undef __TEUTHID_UNSIGNED_INTEGER_FROM_STRING

#ifdef TEUTHID_HAVE_INT_128
uint128_t system::string_to_unit128_(const std::string &s) {
  uint128_t __value = 0;
  for (std::size_t __i = 0; __i < s.size(); __i++)
    if (std::isdigit(s[__i])) {
      __value += std::stoi(s.substr(__i, 1));
      if ((__i + 1) < s.size())
        __value *= 10;
    } else
      throw std::invalid_argument("invalid string");
  return __value;
}

template <> int128_t &system::from_string(const std::string &s, int128_t &x) {
  bool __minus = false;
  std::string __s = system::validate_string_(s);
  if (!__s.empty()) {
    if ((__s[0] == '+') || (__s[0] == '-')) {
      __minus = (__s[0] == '-');
      __s = __s.substr(1);
    }
    if (!__s.empty()) {
      x = system::string_to_unit128_(__s);
      x = __minus ? -x : x;
      return x;
    }
  }
  throw std::invalid_argument("empty or invalid string");
}

template <> uint128_t &system::from_string(const std::string &s, uint128_t &x) {
  std::string __s = system::validate_string_(s);
  if (!__s.empty()) {
    if (__s[0] == '+')
      __s = __s.substr(1);
    if (!__s.empty()) {
      x = system::string_to_unit128_(__s);
      return x;
    }
  }
  throw std::invalid_argument("empty or invalid string");
}
#endif // TEUTHID_HAVE_INT_128

#define __TEUTHID_FLOAT_FROM_STRING(TYPE, FUN)                                 \
  template <> TYPE &system::from_string(const std::string &s, TYPE &x) {       \
    std::string __s = system::validate_string_(s);                             \
    if (!__s.empty()) {                                                        \
      x = FUN(__s);                                                            \
      return x;                                                                \
    }                                                                          \
    throw std::invalid_argument("empty or invalid string");                    \
  }
__TEUTHID_FLOAT_FROM_STRING(float, stof);
__TEUTHID_FLOAT_FROM_STRING(double, stod);
__TEUTHID_FLOAT_FROM_STRING(long double, stold);
#undef __TEUTHID_FLOAT_FROM_STRING

template <> mpfr_t &system::from_string(const std::string &s, mpfr_t &x) {
  std::string __s = system::validate_string_(s);
  if (!__s.empty())
    if (mpfr_set_str(x, __s.c_str(), 10, mpfr_get_default_rounding_mode()) == 0)
      return x;
  throw std::invalid_argument("empty or invalid string");
}

template <>
floatmp_base &system::from_string(const std::string &s, floatmp_base &x) {
  std::string __s = system::validate_string_(s);
  mpfr_t __result;
  mpfr_init2(__result, mpfr_get_prec(x.c_mpfr()));
  if (!__s.empty())
    if (mpfr_set_str(__result, __s.c_str(), 10,
                     static_cast<mpfr_rnd_t>(floatmp_base::rounding_mode())) ==
        0) {
      x.assign(__result);
      mpfr_clear(__result);
      return x;
    }
  throw std::invalid_argument("empty or invalid string");
}

#define __TEUTHID_FLOAT_EQUAL_TO(TYPE)                                         \
  template <> bool system::equal_to(const TYPE &x, const TYPE &y) {            \
    if (std::isfinite(x) && std::isfinite(y))                                  \
      return (system::to_string(x).compare(system::to_string(y)) == 0);        \
    else                                                                       \
      return false;                                                            \
  }
__TEUTHID_FLOAT_EQUAL_TO(float);
__TEUTHID_FLOAT_EQUAL_TO(double);
__TEUTHID_FLOAT_EQUAL_TO(long double);
#undef __TEUTHID_FLOAT_EQUAL_TO

template <> bool system::equal_to(const mpfr_t &x, const mpfr_t &y) {
  if ((mpfr_number_p(x) == 0) || (mpfr_number_p(y) == 0))
    return false;
  else
    return (system::to_string(x).compare(system::to_string(y)) == 0);
}

#define __TEUTHID_FLOAT_LESS_THAN(TYPE)                                        \
  template <> bool system::less_than(const TYPE &x, const TYPE &y) {           \
    if (std::isfinite(x) && std::isfinite(y))                                  \
      if (system::to_string(x).compare(system::to_string(y)) != 0)             \
        return (x < y);                                                        \
    return false;                                                              \
  }
__TEUTHID_FLOAT_LESS_THAN(float);
__TEUTHID_FLOAT_LESS_THAN(double);
__TEUTHID_FLOAT_LESS_THAN(long double);
#undef __TEUTHID_FLOAT_LESS_THAN

template <> bool system::less_than(const mpfr_t &x, const mpfr_t &y) {
  if ((mpfr_number_p(x) != 0) && (mpfr_number_p(y) != 0))
    if (system::to_string(x).compare(system::to_string(y)) != 0)
      return (mpfr_less_p(x, y) != 0);
  return false;
}

#endif // DOXYGEN_SHOULD_SKIP_THIS
