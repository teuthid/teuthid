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
#include <utility>
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

  template <typename T> static std::string to_string(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  static std::size_t split_string(const std::string &s,
                                  std::vector<std::string> &v,
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
  template <typename T> static T &from_string(const std::string &s, T &x) {
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
  template <typename T> static bool is_finite(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_infinite(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_nan(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_zero(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_positive(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static bool is_negative(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> static void swap(T &x, T &y) { std::swap(x, y); }
  template <typename T> static constexpr bool is_floatmp(const T &) {
    return false;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static std::string to_string(const bool &x) {
    return (x ? std::string("true") : std::string("false"));
  }
  static std::string to_string(const int8_t &x) { return std::to_string(x); }
  static std::string to_string(const int16_t &x) { return std::to_string(x); }
  static std::string to_string(const int32_t &x) { return std::to_string(x); }
  static std::string to_string(const int64_t &x) { return std::to_string(x); }
  static std::string to_string(const uint8_t &x) { return std::to_string(x); }
  static std::string to_string(const uint16_t &x) { return std::to_string(x); }
  static std::string to_string(const uint32_t &x) { return std::to_string(x); }
  static std::string to_string(const uint64_t &x) { return std::to_string(x); }
  static std::string to_string(const char &x) { return std::string(1, x); }
  static std::string to_string(const char *const &x) { return std::string(x); }
  static std::string to_string(const std::string &x) { return std::string(x); }
  static std::string to_string(void *const &x) {
    return system::to_string(reinterpret_cast<uintptr_t>(x));
  }
  template <std::size_t P> static std::string to_string(const floatmp<P> &x) {
    return system::to_string(x.c_mpfr());
  }
  template <std::size_t P>
  static floatmp<P> &from_string(const std::string &s, floatmp<P> &x);
  template <std::size_t P> static bool is_finite(const floatmp<P> &x) {
    return x.is_finite();
  }
  template <std::size_t P> static bool is_infinite(const floatmp<P> &x) {
    return x.is_infinite();
  }
  template <std::size_t P> static bool is_nan(const floatmp<P> &x) {
    return x.is_nan();
  }
  template <std::size_t P> static bool is_zero(const floatmp<P> &x) {
    return x.is_zero();
  }
  template <std::size_t P> static bool is_positive(const floatmp<P> &x) {
    return x.is_positive();
  }
  template <std::size_t P> static bool is_negative(const floatmp<P> &x) {
    return x.is_negative();
  }
  template <std::size_t P1, std::size_t P2>
  static void swap(floatmp<P1> &x, floatmp<P2> &y) {
    x.swap(y);
  }
  template <std::size_t P>
  static constexpr bool is_floatmp(const floatmp<P> &) {
    return true;
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
  static std::string uint128_to_string_(uint128_t x);
  static uint128_t string_to_unit128_(const std::string &s);
#endif
  static std::string validate_string_(const std::string &s);
}; // class system

#ifndef DOXYGEN_SHOULD_SKIP_THIS

// specializations of system::to_string<T>()
#ifdef TEUTHID_HAVE_INT_128
template <> std::string system::to_string(const int128_t &x);
template <> std::string system::to_string(const uint128_t &x);
#endif // TEUTHID_HAVE_INT_128
template <> std::string system::to_string(const float &x);
template <> std::string system::to_string(const double &x);
template <> std::string system::to_string(const long double &x);
template <> std::string system::to_string(const mpfr_t &x);
template <> std::string system::to_string(const floatmp_base &x);
template <> std::string system::to_string(const std::vector<std::string> &v);

// specializations of system::from_string<T>()
// may throw: std::invalid_argument, std::out_of_range
template <> bool &system::from_string(const std::string &s, bool &x);
template <> int8_t &system::from_string(const std::string &s, int8_t &x);
template <> int16_t &system::from_string(const std::string &s, int16_t &x);
template <> int32_t &system::from_string(const std::string &s, int32_t &x);
template <> int64_t &system::from_string(const std::string &s, int64_t &x);
template <> uint8_t &system::from_string(const std::string &s, uint8_t &x);
template <> uint16_t &system::from_string(const std::string &s, uint16_t &x);
template <> uint32_t &system::from_string(const std::string &s, uint32_t &x);
template <> uint64_t &system::from_string(const std::string &s, uint64_t &x);
template <> float &system::from_string(const std::string &s, float &x);
template <> double &system::from_string(const std::string &s, double &x);
template <>
long double &system::from_string(const std::string &s, long double &x);
template <> mpfr_t &system::from_string(const std::string &s, mpfr_t &x);
template <>
floatmp_base &system::from_string(const std::string &s, floatmp_base &x);
template <std::size_t P>
floatmp<P> &system::from_string(const std::string &s, floatmp<P> &x) {
  return dynamic_cast<floatmp<P> &>(
      system::from_string(s, static_cast<floatmp_base &>(x)));
}
#ifdef TEUTHID_HAVE_INT_128
template <> int128_t &system::from_string(const std::string &s, int128_t &x);
template <> uint128_t &system::from_string(const std::string &s, uint128_t &x);
#endif // TEUTHID_HAVE_INT_128

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
  template <> inline bool system::is_finite(const TYPE &x) {                   \
    return std::isfinite(x);                                                   \
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
template <> inline bool system::is_finite(const mpfr_t &x) {
  return (mpfr_number_p(x) != 0);
}
template <> inline bool system::is_finite(const floatmp_base &x) {
  return x.is_finite();
}
#ifdef TEUTHID_HAVE_INT_128
template <> inline bool system::is_finite(const int128_t &x) {
  return system::is_finite(floatmp_base::int128_to_ldouble_(x));
}
template <> inline bool system::is_finite(const uint128_t &x) {
  return system::is_finite(floatmp_base::uint128_to_ldouble_(x));
}
#endif // TEUTHID_HAVE_INT_128

// specialization of system::is_infinite<T>()
#define __TEUTHID_SYSTEM_IS_INFINITE(TYPE)                                     \
  template <> inline bool system::is_infinite(const TYPE &x) {                 \
    return std::isinf(x);                                                      \
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
template <> inline bool system::is_infinite(const mpfr_t &x) {
  return (mpfr_inf_p(x) != 0);
}
template <> inline bool system::is_infinite(const floatmp_base &x) {
  return x.is_infinite();
}
#ifdef TEUTHID_HAVE_INT_128
template <> inline bool system::is_infinite(const int128_t &x) {
  return system::is_infinite(floatmp_base::int128_to_ldouble_(x));
}
template <> inline bool system::is_infinite(const uint128_t &x) {
  return system::is_infinite(floatmp_base::uint128_to_ldouble_(x));
}
#endif // TEUTHID_HAVE_INT_128

// specialization of system::is_nan<T>()
#define __TEUTHID_SYSTEM_IS_NAN(TYPE)                                          \
  template <> inline bool system::is_nan(const TYPE &x) {                      \
    return std::isnan(x);                                                      \
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
template <> inline bool system::is_nan(const mpfr_t &x) {
  return (mpfr_nan_p(x) != 0);
}
template <> inline bool system::is_nan(const floatmp_base &x) {
  return x.is_nan();
}
#ifdef TEUTHID_HAVE_INT_128
template <> inline bool system::is_nan(const int128_t &x) {
  return system::is_nan(floatmp_base::int128_to_ldouble_(x));
}
template <> inline bool system::is_nan(const uint128_t &x) {
  return system::is_nan(floatmp_base::uint128_to_ldouble_(x));
}
#endif // TEUTHID_HAVE_INT_128

// specialization of system::is_zero<T>()
template <> inline bool system::is_zero(const bool &x) { return !x; }
template <> inline bool system::is_zero(const char &x) { return !x; }
template <> inline bool system::is_zero(const int8_t &x) { return !x; }
template <> inline bool system::is_zero(const int16_t &x) { return !x; }
template <> inline bool system::is_zero(const int32_t &x) { return !x; }
template <> inline bool system::is_zero(const int64_t &x) { return !x; }
template <> inline bool system::is_zero(const uint8_t &x) { return !x; }
template <> inline bool system::is_zero(const uint16_t &x) { return !x; }
template <> inline bool system::is_zero(const uint32_t &x) { return !x; }
template <> inline bool system::is_zero(const uint64_t &x) { return !x; }
#ifdef TEUTHID_HAVE_INT_128
template <> inline bool system::is_zero(const int128_t &x) { return !x; }
template <> inline bool system::is_zero(const uint128_t &x) { return !x; }
#endif // TEUTHID_HAVE_INT_128
template <> inline bool system::is_zero(const float &x) {
  return system::equal_to(x, static_cast<float>(0));
}
template <> inline bool system::is_zero(const double &x) {
  return system::equal_to(x, static_cast<double>(0));
}
template <> inline bool system::is_zero(const long double &x) {
  return system::equal_to(x, static_cast<long double>(0));
}
template <> inline bool system::is_zero(const mpfr_t &x) {
  return system::equal_to(mpfr_get_ld(x, mpfr_get_default_rounding_mode()),
                          static_cast<long double>(0));
}

// specialization of system::is_positive<T>()
template <> inline bool system::is_positive(const bool &x) { return x; }
template <> inline bool system::is_positive(const char &x) { return x > 0; }
template <> inline bool system::is_positive(const int8_t &x) { return x > 0; }
template <> inline bool system::is_positive(const int16_t &x) { return x > 0; }
template <> inline bool system::is_positive(const int32_t &x) { return x > 0; }
template <> inline bool system::is_positive(const int64_t &x) { return x > 0; }
template <> inline bool system::is_positive(const uint8_t &x) { return x > 0; }
template <> inline bool system::is_positive(const uint16_t &x) { return x > 0; }
template <> inline bool system::is_positive(const uint32_t &x) { return x > 0; }
template <> inline bool system::is_positive(const uint64_t &x) { return x > 0; }
#ifdef TEUTHID_HAVE_INT_128
template <> inline bool system::is_positive(const int128_t &x) { return x > 0; }
template <> inline bool system::is_positive(const uint128_t &x) {
  return x > 0;
}
#endif // TEUTHID_HAVE_INT_128
template <> inline bool system::is_positive(const float &x) {
  return system::less_than(static_cast<float>(0), x);
}
template <> inline bool system::is_positive(const double &x) {
  return system::less_than(static_cast<double>(0), x);
}
template <> inline bool system::is_positive(const long double &x) {
  return system::less_than(static_cast<long double>(0), x);
}
template <> inline bool system::is_positive(const mpfr_t &x) {
  return system::less_than(static_cast<long double>(0),
                           mpfr_get_ld(x, mpfr_get_default_rounding_mode()));
}

// specialization of system::is_negative<T>()
template <> inline bool system::is_negative(const bool &x) { return false; }
template <> inline bool system::is_negative(const char &x) { return x < 0; }
template <> inline bool system::is_negative(const int8_t &x) { return x < 0; }
template <> inline bool system::is_negative(const int16_t &x) { return x < 0; }
template <> inline bool system::is_negative(const int32_t &x) { return x < 0; }
template <> inline bool system::is_negative(const int64_t &x) { return x < 0; }
template <> inline bool system::is_negative(const uint8_t &x) { return false; }
template <> inline bool system::is_negative(const uint16_t &x) { return false; }
template <> inline bool system::is_negative(const uint32_t &x) { return false; }
template <> inline bool system::is_negative(const uint64_t &x) { return false; }
#ifdef TEUTHID_HAVE_INT_128
template <> inline bool system::is_negative(const int128_t &x) { return x < 0; }
template <> inline bool system::is_negative(const uint128_t &x) {
  return false;
}
#endif // TEUTHID_HAVE_INT_128
template <> inline bool system::is_negative(const float &x) {
  return system::less_than(x, static_cast<float>(0));
}
template <> inline bool system::is_negative(const double &x) {
  return system::less_than(x, static_cast<double>(0));
}
template <> inline bool system::is_negative(const long double &x) {
  return system::less_than(x, static_cast<long double>(0));
}
template <> inline bool system::is_negative(const mpfr_t &x) {
  return system::less_than(mpfr_get_ld(x, mpfr_get_default_rounding_mode()),
                           static_cast<long double>(0));
}

// specialization of system::swap<T>()
template <> inline void system::swap(mpfr_t &x, mpfr_t &y) { mpfr_swap(x, y); }

#endif // DOXYGEN_SHOULD_SKIP_THIS

} // namespace teuthid

#endif // TEUTHID_SYSTEM_HPP
