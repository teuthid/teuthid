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

#include <ios>
#include <mutex>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <teuthid/config.hpp>

namespace teuthid {

class system final {
public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  system(system const &) = delete;
  void operator=(system const &) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS
  static constexpr uint8_t major_version() noexcept {
    return TEUTHID_MAJOR_VERSION;
  }
  static constexpr uint8_t minor_version() noexcept {
    return TEUTHID_MINOR_VERSION;
  }
  static constexpr uint8_t patch_version() noexcept {
    return TEUTHID_PATCH_VERSION;
  }
  static constexpr uint8_t soversion() noexcept { return TEUTHID_SOVERSION; }
  static const std::string &version() noexcept { return system::version_; }
  static bool is_required_version(uint8_t major, uint8_t minor) noexcept;
  static bool have_clb();
  static bool use_clb() noexcept { return system::use_clb_; }
  static bool use_clb(bool enabled);

  template <typename T> static std::string to_string(const T &value);
  static void format_float_output(std::streamsize precision,
                                  bool scientific = false);
  template <typename T>
  static T &from_string(const std::string &str_value, T &value);

private:
  system() {}
  ~system() {}
  static std::string version_;
  static thread_local bool use_clb_;
  static std::mutex mutex_;
  static std::streamsize float_precision_;
  static bool float_scientific_format_;
};

template <typename T> std::string system::to_string(const T &value) {
  static_assert(!std::is_pointer<T>::value, "requires non-pointer type");
  static_assert(!std::is_array<T>::value, "requires non-array type");
  std::ostringstream __os;
  __os.precision(system::float_precision_);
  __os << (system::float_scientific_format_ ? std::scientific : std::fixed);
  __os << value;
  return __os.str();
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// specializations of system::to_string<T>()
template <> std::string system::to_string(const bool &value) {
  return (value ? std::string("true") : std::string("false"));
}
template <> std::string system::to_string(const int8_t &value);
template <> std::string system::to_string(const int16_t &value);
template <> std::string system::to_string(const int32_t &value);
template <> std::string system::to_string(const int64_t &value);
template <> std::string system::to_string(const uint8_t &value);
template <> std::string system::to_string(const uint16_t &value);
template <> std::string system::to_string(const uint32_t &value);
template <> std::string system::to_string(const uint64_t &value);
#ifdef TEUTHID_HAVE_INT_128
template <> std::string system::to_string(const int128_t &value);
template <> std::string system::to_string(const uint128_t &value);
#endif // TEUTHID_HAVE_INT_128
template <> std::string system::to_string(const char &value) {
  return std::string(1, value);
}
template <> std::string system::to_string(const char *const &value) {
  return std::string(value);
}
template <> std::string system::to_string(const std::string &value) {
  return std::string(value);
}
template <>
std::string system::to_string(const std::vector<std::string> &value);
template <> std::string system::to_string(void *const &value) {
  return system::to_string(reinterpret_cast<uintptr_t>(value));
}
template <> std::string system::to_string(const mpfr_t &value);

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
#endif // DOXYGEN_SHOULD_SKIP_THIS

} // namespace teuthid

#endif // TEUTHID_SYSTEM_HPP
