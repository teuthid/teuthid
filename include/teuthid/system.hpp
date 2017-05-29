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
};

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
#endif // DOXYGEN_SHOULD_SKIP_THIS

} // namespace teuthid

#endif // TEUTHID_SYSTEM_HPP
