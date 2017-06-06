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

#ifndef TEUTHID_SYSTEM_HPP
#define TEUTHID_SYSTEM_HPP

#include <atomic>
#include <cmath>
#include <string>
#include <vector>

#include <teuthid/floatmp.hpp>

namespace teuthid {

class system final {
public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  system(const system &) = delete;
  system(system &&) = delete;
  system &operator=(const system &) = delete;
  system &operator=(system &&) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS
  static constexpr uint8_t major_version() { return TEUTHID_MAJOR_VERSION; }
  static constexpr uint8_t minor_version() { return TEUTHID_MINOR_VERSION; }
  static constexpr uint8_t patch_version() { return TEUTHID_PATCH_VERSION; }
  static constexpr uint8_t soversion() noexcept { return TEUTHID_SOVERSION; }
  static const std::string &version() noexcept { return system::version_; }
  static bool check_version(uint8_t major, uint8_t minor) noexcept;
  static bool has_cl_backend();
  static bool uses_cl_backend() noexcept { return system::clb_.load(); }
  static bool uses_cl_backend(bool enabled);

  template <typename T> static std::string to_string(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  static std::size_t split_string(const std::string &str,
                                  std::vector<std::string> &vec,
                                  char delim = ' ');
  static std::streamsize default_format_float_precision() noexcept {
    return default_format_float_precision_;
  }
  static void format_float_output(
      std::streamsize precision = system::default_format_float_precision(),
      bool scientific = false);
  static std::streamsize format_float_precision(std::streamsize precision);
  static bool format_float_scientific(bool scientific) {
    return format_float_scientific_.exchange(scientific);
  }
  template <typename T>
  static T &from_string(const std::string &str_value, T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool equal_to(const T &x, const T &y) {
    return (x == y);
  }
  template <typename T> static bool less_than(const T &x, const T &y) {
    return (x < y);
  }
  template <typename E> static constexpr bool test_enumerator(E en) noexcept {
    static_assert(std::is_enum<E>::value, "requires enumeration type");
    return static_cast<typename std::underlying_type<E>::type>(en);
  }
  template <typename T> static bool is_finite(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_infinite(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_nan(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_zero(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static std::string to_string(const bool &value) {
    return (value ? std::string("true") : std::string("false"));
  }
  static std::string to_string(const int8_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const int16_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const int32_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const int64_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const uint8_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const uint16_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const uint32_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const uint64_t &value) {
    return std::to_string(value);
  }
  static std::string to_string(const char &value) {
    return std::string(1, value);
  }
  static std::string to_string(const char *const &value) {
    return std::string(value);
  }
  static std::string to_string(const std::string &value) {
    return std::string(value);
  }
  static std::string to_string(void *const &value) {
    return system::to_string(reinterpret_cast<uintptr_t>(value));
  }
  template <std::size_t Precision>
  static std::string to_string(const floatmp<Precision> &value) {
    return system::to_string(value.c_mpfr());
  }
  template <std::size_t Precision>
  static floatmp<Precision> &from_string(const std::string &str_value,
                                         floatmp<Precision> &value);
  template <std::size_t Precision>
  static bool is_finite(const floatmp<Precision> &value) {
    return value.is_finite();
  }
  template <std::size_t Precision>
  static bool is_infinite(const floatmp<Precision> &value) {
    return value.is_infinite();
  }
  template <std::size_t Precision>
  static bool is_nan(const floatmp<Precision> &value) {
    return value.is_nan();
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS

private:
  system() {}
  ~system() {}
  static std::string version_;
  static std::atomic_bool clb_;
  static constexpr std::streamsize default_format_float_precision_ = 10;
  static std::atomic<std::streamsize> format_float_precision_;
  static std::atomic_bool format_float_scientific_;
#ifdef TEUTHID_HAVE_INT_128
  static std::string uint128_to_string_(uint128_t value);
#endif
  static std::string validate_string_(const std::string &str);
}; // class system

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// specializations of system::to_string<T>()
#ifdef TEUTHID_HAVE_INT_128
template <> std::string system::to_string(const int128_t &value);
template <> std::string system::to_string(const uint128_t &value);
#endif // TEUTHID_HAVE_INT_128
template <> std::string system::to_string(const float &value);
template <> std::string system::to_string(const double &value);
template <> std::string system::to_string(const long double &value);
template <> std::string system::to_string(const mpfr_t &value);
template <> std::string system::to_string(const floatmp_base &value);
template <>
std::string system::to_string(const std::vector<std::string> &value);

// specializations of system::from_string<T>()
// may throw: std::invalid_argument, std::out_of_range
template <>
bool &system::from_string(const std::string &str_value, bool &value);
template <>
int8_t &system::from_string(const std::string &str_value, int8_t &value);
template <>
int16_t &system::from_string(const std::string &str_value, int16_t &value);
template <>
int32_t &system::from_string(const std::string &str_value, int32_t &value);
template <>
int64_t &system::from_string(const std::string &str_value, int64_t &value);
template <>
uint8_t &system::from_string(const std::string &str_value, uint8_t &value);
template <>
uint16_t &system::from_string(const std::string &str_value, uint16_t &value);
template <>
uint32_t &system::from_string(const std::string &str_value, uint32_t &value);
template <>
uint64_t &system::from_string(const std::string &str_value, uint64_t &value);
template <>
float &system::from_string(const std::string &str_value, float &value);
template <>
double &system::from_string(const std::string &str_value, double &value);
template <>
long double &system::from_string(const std::string &str_value,
                                 long double &value);
template <>
mpfr_t &system::from_string(const std::string &str_value, mpfr_t &value);
template <>
floatmp_base &system::from_string(const std::string &str_value,
                                  floatmp_base &value);
template <std::size_t Precision>
floatmp<Precision> &system::from_string(const std::string &str_value,
                                        floatmp<Precision> &value) {
  return dynamic_cast<floatmp<Precision> &>(
      system::from_string(str_value, static_cast<floatmp_base &>(value)));
}
// specializations of system::equal_to<T>()
template <> bool system::equal_to(const float &x, const float &y);
template <> bool system::equal_to(const double &x, const double &y);
template <> bool system::equal_to(const long double &x, const long double &y);
template <> bool system::equal_to(const mpfr_t &x, const mpfr_t &y);

// specializations of system::less_than<T>()
template <> bool system::less_than(const float &x, const float &y);
template <> bool system::less_than(const double &x, const double &y);
template <> bool system::less_than(const long double &x, const long double &y);
template <> bool system::less_than(const mpfr_t &x, const mpfr_t &y);

// specialization of system::is_finite<T>()
#define __TEUTHID_SYSTEM_IS_FINITE(TYPE)                                       \
  template <> inline bool system::is_finite(const TYPE &value) {               \
    return std::isfinite(value);                                               \
  }
__TEUTHID_SYSTEM_IS_FINITE(bool)
__TEUTHID_SYSTEM_IS_FINITE(char)
__TEUTHID_SYSTEM_IS_FINITE(int8_t)
__TEUTHID_SYSTEM_IS_FINITE(int16_t)
__TEUTHID_SYSTEM_IS_FINITE(int32_t)
__TEUTHID_SYSTEM_IS_FINITE(int64_t)
__TEUTHID_SYSTEM_IS_FINITE(uint8_t)
__TEUTHID_SYSTEM_IS_FINITE(uint16_t)
__TEUTHID_SYSTEM_IS_FINITE(uint32_t)
__TEUTHID_SYSTEM_IS_FINITE(uint64_t)
__TEUTHID_SYSTEM_IS_FINITE(float)
__TEUTHID_SYSTEM_IS_FINITE(double)
__TEUTHID_SYSTEM_IS_FINITE(long double)
#undef __TEUTHID_SYSTEM_IS_FINITE
template <> inline bool system::is_finite(const mpfr_t &value) {
  return (mpfr_number_p(value) != 0);
}
template <> inline bool system::is_finite(const floatmp_base &value) {
  return value.is_finite();
}

// specialization of system::is_infinite<T>()
#define __TEUTHID_SYSTEM_IS_INFINITE(TYPE)                                     \
  template <> inline bool system::is_infinite(const TYPE &value) {             \
    return std::isinf(value);                                                  \
  }
__TEUTHID_SYSTEM_IS_INFINITE(bool)
__TEUTHID_SYSTEM_IS_INFINITE(char)
__TEUTHID_SYSTEM_IS_INFINITE(int8_t)
__TEUTHID_SYSTEM_IS_INFINITE(int16_t)
__TEUTHID_SYSTEM_IS_INFINITE(int32_t)
__TEUTHID_SYSTEM_IS_INFINITE(int64_t)
__TEUTHID_SYSTEM_IS_INFINITE(uint8_t)
__TEUTHID_SYSTEM_IS_INFINITE(uint16_t)
__TEUTHID_SYSTEM_IS_INFINITE(uint32_t)
__TEUTHID_SYSTEM_IS_INFINITE(uint64_t)
__TEUTHID_SYSTEM_IS_INFINITE(float)
__TEUTHID_SYSTEM_IS_INFINITE(double)
__TEUTHID_SYSTEM_IS_INFINITE(long double)
#undef __TEUTHID_SYSTEM_IS_INFINITE
template <> inline bool system::is_infinite(const mpfr_t &value) {
  return (mpfr_inf_p(value) != 0);
}
template <> inline bool system::is_infinite(const floatmp_base &value) {
  return value.is_infinite();
}

// specialization of system::is_nan<T>()
#define __TEUTHID_SYSTEM_IS_NAN(TYPE)                                          \
  template <> inline bool system::is_nan(const TYPE &value) {                  \
    return std::isnan(value);                                                  \
  }
__TEUTHID_SYSTEM_IS_NAN(bool)
__TEUTHID_SYSTEM_IS_NAN(char)
__TEUTHID_SYSTEM_IS_NAN(int8_t)
__TEUTHID_SYSTEM_IS_NAN(int16_t)
__TEUTHID_SYSTEM_IS_NAN(int32_t)
__TEUTHID_SYSTEM_IS_NAN(int64_t)
__TEUTHID_SYSTEM_IS_NAN(uint8_t)
__TEUTHID_SYSTEM_IS_NAN(uint16_t)
__TEUTHID_SYSTEM_IS_NAN(uint32_t)
__TEUTHID_SYSTEM_IS_NAN(uint64_t)
__TEUTHID_SYSTEM_IS_NAN(float)
__TEUTHID_SYSTEM_IS_NAN(double)
__TEUTHID_SYSTEM_IS_NAN(long double)
#undef __TEUTHID_SYSTEM_IS_NAN
template <> inline bool system::is_nan(const mpfr_t &value) {
  return (mpfr_nan_p(value) != 0);
}
template <> inline bool system::is_nan(const floatmp_base &value) {
  return value.is_nan();
}

// specialization of system::is_zero<T>()
template <> inline bool system::is_zero(const bool &value) { return !value; }
template <> inline bool system::is_zero(const char &value) { return !value; }
template <> inline bool system::is_zero(const int8_t &value) { return !value; }
template <> inline bool system::is_zero(const int16_t &value) { return !value; }
template <> inline bool system::is_zero(const int32_t &value) { return !value; }
template <> inline bool system::is_zero(const int64_t &value) { return !value; }
template <> inline bool system::is_zero(const uint8_t &value) { return !value; }
template <> inline bool system::is_zero(const uint16_t &value) {
  return !value;
}
template <> inline bool system::is_zero(const uint32_t &value) {
  return !value;
}
template <> inline bool system::is_zero(const uint64_t &value) {
  return !value;
}
template <> inline bool system::is_zero(const float &value) {
  return system::equal_to(value, static_cast<float>(0));
}
template <> inline bool system::is_zero(const double &value) {
  return system::equal_to(value, static_cast<double>(0));
}
template <> inline bool system::is_zero(const long double &value) {
  return system::equal_to(value, static_cast<long double>(0));
}

#endif // DOXYGEN_SHOULD_SKIP_THIS

} // namespace teuthid

#endif // TEUTHID_SYSTEM_HPP
