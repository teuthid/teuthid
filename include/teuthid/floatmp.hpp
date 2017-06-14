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

#ifndef TEUTHID_FLOATMP_HPP
#define TEUTHID_FLOATMP_HPP

#include <algorithm>
#include <atomic>

#include <mpfr.h>
#include <teuthid/config.hpp>

// forward declarations:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace teuthid {
class system;
template <std::size_t Precision> class floatmp;
}
namespace std {
template <std::size_t P1, std::size_t P2>
auto fmax(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y);
template <std::size_t P1, std::size_t P2>
auto fmin(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y);
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace teuthid {

enum class floatmp_round_t : int {
  RNDN = MPFR_RNDN, // round to nearest, with ties to even
  RNDZ = MPFR_RNDZ, // round toward zero
  RNDU = MPFR_RNDU, // round toward +Inf
  RNDD = MPFR_RNDD, // round toward -Inf
  RNDA = MPFR_RNDA, // round away from zero
  // RNDF = MPFR_RNDF,  // faithful rounding (not implemented yet)
  RNDNA = MPFR_RNDNA // round to nearest, with ties away from zero
};

#define TEUTHID_CHECK_FLOATMP_PRECISION(PRECISION)                             \
  static_assert((PRECISION >= floatmp_base::min_precision()),                  \
                "Too low floatmp precision.");                                 \
  static_assert((PRECISION < floatmp_base::max_precision()),                   \
                "Too high floatmp precision.");

class floatmp_base {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  friend class system;
  template <std::size_t Precision> friend class floatmp;
  template <std::size_t P1, std::size_t P2>
  friend auto std::fmax(const teuthid::floatmp<P1> &x,
                        const teuthid::floatmp<P2> &y);
  template <std::size_t P1, std::size_t P2>
  friend auto std::fmin(const teuthid::floatmp<P1> &x,
                        const teuthid::floatmp<P2> &y);
#endif // DOXYGEN_SHOULD_SKIP_THIS

public:
  floatmp_base(std::size_t precision) {
    mpfr_init2(value_, precision);
    mpfr_set_zero(value_, 1);
  }
  floatmp_base(std::size_t precision, const floatmp_base &value) {
    mpfr_init2(value_, precision);
    mpfr_set(value_, value.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  template <typename T> floatmp_base(std::size_t precision, const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  virtual ~floatmp_base() { mpfr_clear(value_); }
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  floatmp_base &operator=(const floatmp_base &) = delete;
  floatmp_base &operator=(floatmp_base &&) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS

  explicit operator float() const {
    return mpfr_get_flt(value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  explicit operator double() const {
    return mpfr_get_d(value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  explicit operator long double() const {
    return mpfr_get_ld(value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }

  template <typename T> void assign(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void add(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void sub(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void mul(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void div(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  const mpfr_t &c_mpfr() const noexcept { return value_; }
  bool is_finite() const { return (mpfr_number_p(value_) != 0); }
  bool is_infinite() const { return (mpfr_inf_p(value_) != 0); }
  bool is_nan() const { return (mpfr_nan_p(value_) != 0); }
  bool is_zero() const {
    return equal_to(floatmp_base(mpfr_get_prec(value_), 0));
  }

  static constexpr std::size_t max_precision() noexcept {
    return MPFR_PREC_MAX;
  }
  static constexpr std::size_t min_precision() noexcept {
    return MPFR_PREC_MIN;
  }
  static floatmp_round_t rounding_mode() {
    return static_cast<floatmp_round_t>(round_mode_.load());
  }
  static floatmp_round_t rounding_mode(floatmp_round_t mode) {
    return static_cast<floatmp_round_t>(
        round_mode_.exchange(static_cast<int>(mode)));
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __TEUTHID_FLOATMP_ASSIGN_SPEC(TYPE, FUN)                               \
  floatmp_base(std::size_t precision, const TYPE &value) {                     \
    mpfr_init2(value_, precision);                                             \
    FUN(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));              \
  }                                                                            \
  void assign(const TYPE &value) {                                             \
    FUN(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));              \
  }
  __TEUTHID_FLOATMP_ASSIGN_SPEC(int8_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(int16_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(int32_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(int64_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(uint8_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(uint16_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(uint32_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(uint64_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(float, mpfr_set_flt)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(double, mpfr_set_d)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(long double, mpfr_set_ld)
  __TEUTHID_FLOATMP_ASSIGN_SPEC(mpfr_t, mpfr_set)
#undef __TEUTHID_FLOATMP_ASSIGN_SPEC
  template <std::size_t P> void assign(const floatmp<P> &value) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_set(value_, value.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }

#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER, TYPE, FUN)                     \
  void OPER(const TYPE &value) {                                               \
    FUN(value_, c_mpfr(), value, static_cast<mpfr_rnd_t>(rounding_mode()));    \
  }
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, int8_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, int16_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, int32_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, int64_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, uint8_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, uint16_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, uint32_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, uint64_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, float, mpfr_add_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, double, mpfr_add_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, mpfr_t, mpfr_add)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, int8_t, mpfr_sub_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, int16_t, mpfr_sub_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, int32_t, mpfr_sub_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, int64_t, mpfr_sub_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, uint8_t, mpfr_sub_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, uint16_t, mpfr_sub_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, uint32_t, mpfr_sub_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, uint64_t, mpfr_sub_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, float, mpfr_sub_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, double, mpfr_sub_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, mpfr_t, mpfr_sub)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, int8_t, mpfr_mul_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, int16_t, mpfr_mul_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, int32_t, mpfr_mul_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, int64_t, mpfr_mul_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, uint8_t, mpfr_mul_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, uint16_t, mpfr_mul_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, uint32_t, mpfr_mul_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, uint64_t, mpfr_mul_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, float, mpfr_mul_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, double, mpfr_mul_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, mpfr_t, mpfr_mul)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, int8_t, mpfr_div_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, int16_t, mpfr_div_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, int32_t, mpfr_div_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, int64_t, mpfr_div_si)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, uint8_t, mpfr_div_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, uint16_t, mpfr_div_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, uint32_t, mpfr_div_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, uint64_t, mpfr_div_ui)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, float, mpfr_div_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, double, mpfr_div_d)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, mpfr_t, mpfr_div)
#undef __TEUTHID_FLOATMP_ARITHMETIC_SPEC

#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER, FUN)                           \
  void OPER(const long double &value) {                                        \
    mpfr_t __v;                                                                \
    mpfr_init2(__v, mpfr_get_prec(value_));                                    \
    mpfr_set_ld(__v, value, static_cast<mpfr_rnd_t>(rounding_mode()));         \
    FUN(value_, c_mpfr(), __v, static_cast<mpfr_rnd_t>(rounding_mode()));      \
    mpfr_clear(__v);                                                           \
  }                                                                            \
  template <std::size_t P> void OPER(const floatmp<P> &value) {                \
    TEUTHID_CHECK_FLOATMP_PRECISION(P);                                        \
    FUN(value_, c_mpfr(), value.value_,                                        \
        static_cast<mpfr_rnd_t>(rounding_mode()));                             \
  }
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, mpfr_add)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, mpfr_sub)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, mpfr_mul)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, mpfr_div)
#undef __TEUTHID_FLOATMP_ARITHMETIC_SPEC
#endif // DOXYGEN_SHOULD_SKIP_THIS

private:
  floatmp_base() { mpfr_init2(value_, mpfr_get_default_prec()); }
  floatmp_base(const floatmp_base &value) {
    mpfr_init2(value_, mpfr_get_prec(value.value_));
    mpfr_set(value_, value.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  floatmp_base(floatmp_base &&value) {
    mpfr_init2(value_, mpfr_get_prec(value.value_));
    mpfr_swap(value_, value.value_);
  }

  bool equal_to(const floatmp_base &value) const;
  bool less_than(const floatmp_base &value) const;
  void abs() { // this = abs(this)
    mpfr_abs(value_, c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void fmod(const floatmp_base &divisor); // this = fmod(this, divisor)
  void
  remainder(const floatmp_base &divisor); // this = remainder(this, divisor)
  void fma(const floatmp_base &y, const floatmp_base &z) {
    // this = fma(this, y, z)
    mpfr_fma(value_, c_mpfr(), y.value_, z.value_,
             static_cast<mpfr_rnd_t>(rounding_mode()));
  }

  static const floatmp_base &fmax(const floatmp_base &x, const floatmp_base &y);
  static const floatmp_base &fmin(const floatmp_base &x, const floatmp_base &y);
#ifdef TEUTHID_HAVE_INT_128
  static long double int128_to_ldouble_(const int128_t &value) {
    return static_cast<long double>(INT64_MAX) *
               static_cast<long double>(value / INT64_MAX) +
           static_cast<long double>(value % INT64_MAX);
  }
  static long double uint128_to_ldouble_(const uint128_t &value) {
    return static_cast<long double>(UINT64_MAX) *
               static_cast<long double>(value / UINT64_MAX) +
           static_cast<long double>(value % UINT64_MAX);
  }
#endif // TEUTHID_HAVE_INT_128

  mpfr_t value_;
  static std::atomic_int round_mode_;
}; // class floatmp_base

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef TEUTHID_HAVE_INT_128
template <>
inline floatmp_base::floatmp_base(std::size_t precision,
                                  const int128_t &value) {
  mpfr_init2(value_, precision);
  mpfr_set_ld(value_, int128_to_ldouble_(value),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <>
inline floatmp_base::floatmp_base(std::size_t precision,
                                  const uint128_t &value) {
  mpfr_init2(value_, precision);
  mpfr_set_ld(value_, uint128_to_ldouble_(value),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <> inline void floatmp_base::assign(const int128_t &value) {
  mpfr_set_ld(value_, int128_to_ldouble_(value),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <> inline void floatmp_base::assign(const uint128_t &value) {
  mpfr_set_ld(value_, uint128_to_ldouble_(value),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}

#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER)                                \
  template <> inline void floatmp_base::OPER(const int128_t &value) {          \
    OPER(int128_to_ldouble_(value));                                           \
  }                                                                            \
  template <> inline void floatmp_base::OPER(const uint128_t &value) {         \
    OPER(uint128_to_ldouble_(value));                                          \
  }
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(add)
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub)
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul)
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(div)
#undef __TEUTHID_FLOATMP_ARITHMETIC_SPEC
#endif // TEUTHID_HAVE_INT_128
#endif // DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************/

template <std::size_t Precision> class floatmp : public floatmp_base {
public:
  floatmp() : floatmp_base(Precision) {
    TEUTHID_CHECK_FLOATMP_PRECISION(Precision);
  }
  template <std::size_t P>
  floatmp(const floatmp<P> &value)
      : floatmp_base(Precision, static_cast<const floatmp_base &>(value)) {
    TEUTHID_CHECK_FLOATMP_PRECISION(Precision);
  }
  template <typename T>
  floatmp(const T &value) : floatmp_base(Precision, value) {
    TEUTHID_CHECK_FLOATMP_PRECISION(Precision);
  }
  virtual ~floatmp() {}
  floatmp &operator=(const floatmp &other) {
    if (this != &other)
      mpfr_set(value_, other.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
    return *this;
  }
  template <std::size_t P> floatmp &operator=(const floatmp<P> &other) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_set(value_, other.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
    return *this;
  }
  template <typename T> floatmp &operator=(const T &value) {
    floatmp_base::assign(value);
    return *this;
  }
  template <std::size_t P> operator floatmp<P>() const {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    return floatmp<P>(*this);
  }
  floatmp operator-() const {
    floatmp<Precision> __v(*this);
    mpfr_neg(__v.value_, c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
    return __v;
  }
  template <typename T> floatmp &operator+=(const T &value) {
    floatmp_base::add(value);
    return *this;
  }
  template <typename T> floatmp &operator-=(const T &value) {
    floatmp_base::sub(value);
    return *this;
  }
  template <typename T> floatmp &operator*=(const T &value) {
    floatmp_base::mul(value);
    return *this;
  }
  template <typename T> floatmp &operator/=(const T &value) {
    floatmp_base::div(value);
    return *this;
  }

  template <typename T> floatmp &assign(const T &value) {
    floatmp_base::assign(value);
    return *this;
  }
  template <typename T> bool equal_to(const T &value) const {
    return floatmp_base::equal_to(
        static_cast<floatmp_base>(floatmp<Precision>(value)));
  }
  template <typename T> bool less_than(const T &value) const {
    return floatmp_base::less_than(
        static_cast<floatmp_base>(floatmp<Precision>(value)));
  }
  template <typename T> floatmp &add(const T &value) {
    floatmp_base::add(value);
    return *this;
  }
  template <typename T> floatmp &sub(const T &value) {
    floatmp_base::sub(value);
    return *this;
  }
  template <typename T> floatmp &mul(const T &value) {
    floatmp_base::mul(value);
    return *this;
  }
  template <typename T> floatmp &div(const T &value) {
    floatmp_base::div(value);
    return *this;
  }
  constexpr std::size_t precision() const noexcept { return Precision; }
  template <std::size_t P> floatmp &swap(floatmp<P> &value) {
    floatmp __tmp(value);
    value.assign(value_);
    return assign(__tmp);
  }
  floatmp &abs() { // this = abs(this)
    floatmp_base::abs();
    return *this;
  }
  template <std::size_t P> floatmp &fmod(const floatmp<P> &divisor) {
    // this = fmod(this, divisor)
    floatmp_base::fmod(static_cast<const floatmp_base &>(divisor));
    return *this;
  }
  template <std::size_t P> floatmp &remainder(const floatmp<P> &divisor) {
    // this = remainder(this, divisor)
    floatmp_base::remainder(static_cast<const floatmp_base &>(divisor));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &fma(const floatmp<P1> &y, const floatmp<P2> &z) {
    // this = fma(this, y, z)
    floatmp_base::fma(static_cast<const floatmp_base &>(y),
                      static_cast<const floatmp_base &>(z));
    return *this;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <std::size_t P> bool equal_to(const floatmp<P> &value) const {
    return floatmp_base::equal_to(static_cast<const floatmp_base &>(value));
  }
  template <std::size_t P> bool less_than(const floatmp<P> &value) const {
    return floatmp_base::less_than(static_cast<const floatmp_base &>(value));
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS
};     // class floatmp

/******************************************************************************/

// operator==
template <std::size_t P1, std::size_t P2>
inline bool operator==(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return lhs.equal_to(rhs);
}
template <typename T, std::size_t P>
inline bool operator==(const floatmp<P> &lhs, const T &rhs) {
  return lhs.equal_to(rhs);
}
template <typename T, std::size_t P>
inline bool operator==(const T &lhs, const floatmp<P> &rhs) {
  return rhs.equal_to(lhs);
}

// operator !=
template <std::size_t P1, std::size_t P2>
inline bool operator!=(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return !(lhs == rhs);
}
template <typename T, std::size_t P>
inline bool operator!=(const floatmp<P> &lhs, const T &rhs) {
  return !(lhs == rhs);
}
template <typename T, std::size_t P>
inline bool operator!=(const T &lhs, const floatmp<P> &rhs) {
  return !(lhs == rhs);
}

// operator <
template <std::size_t P1, std::size_t P2>
inline bool operator<(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return lhs.less_than(rhs);
}
template <typename T, std::size_t P>
inline bool operator<(const floatmp<P> &lhs, const T &rhs) {
  return lhs.less_than(rhs);
}
template <typename T, std::size_t P>
inline bool operator<(const T &lhs, const floatmp<P> &rhs) {
  return floatmp<P>(lhs).less_than(rhs);
}

// operator >
template <std::size_t P1, std::size_t P2>
inline bool operator>(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return rhs < lhs;
}
template <typename T, std::size_t P>
inline bool operator>(const floatmp<P> &lhs, const T &rhs) {
  return rhs < lhs;
}
template <typename T, std::size_t P>
inline bool operator>(const T &lhs, const floatmp<P> &rhs) {
  return rhs.less_than(lhs);
}

// operator <=
template <std::size_t P1, std::size_t P2>
inline bool operator<=(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return !(lhs > rhs);
}
template <typename T, std::size_t P>
inline bool operator<=(const floatmp<P> &lhs, const T &rhs) {
  return !(lhs > rhs);
}
template <typename T, std::size_t P>
inline bool operator<=(const T &lhs, const floatmp<P> &rhs) {
  return !(lhs > rhs);
}

// operator >=
template <std::size_t P1, std::size_t P2>
inline bool operator>=(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return !(lhs < rhs);
}
template <typename T, std::size_t P>
inline bool operator>=(const floatmp<P> &lhs, const T &rhs) {
  return !(lhs < rhs);
}
template <typename T, std::size_t P>
inline bool operator>=(const T &lhs, const floatmp<P> &rhs) {
  return !(lhs < rhs);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER, FUN)                           \
  template <std::size_t P1, std::size_t P2>                                    \
  inline auto OPER(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {           \
    return floatmp<std::max(P1, P2)>(lhs).FUN(rhs);                            \
  }                                                                            \
  template <typename T, std::size_t P>                                         \
  inline auto OPER(const floatmp<P> &lhs, const T &rhs) {                      \
    return floatmp<P>(lhs).FUN(rhs);                                           \
  }                                                                            \
  template <typename T, std::size_t P>                                         \
  inline auto OPER(const T &lhs, const floatmp<P> &rhs) {                      \
    return floatmp<P>(lhs).FUN(rhs);                                           \
  }
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(operator+, add)
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(operator-, sub)
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(operator*, mul)
__TEUTHID_FLOATMP_ARITHMETIC_SPEC(operator/, div)
#undef __TEUTHID_FLOATMP_ARITHMETIC_SPEC
#endif // DOXYGEN_SHOULD_SKIP_THIS
} // namespace teuthid

/******************************************************************************/

namespace std {
template <std::size_t P1, std::size_t P2>
inline void swap(teuthid::floatmp<P1> &x, teuthid::floatmp<P2> &y) {
  x.swap(y);
}
template <std::size_t P> inline auto abs(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>(x).abs();
}
template <std::size_t P> inline auto fabs(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>(x).abs();
}
template <std::size_t P1, std::size_t P2>
inline auto fmod(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>(x).fmod(y);
}
template <std::size_t P1, std::size_t P2>
inline auto remainder(const teuthid::floatmp<P1> &x,
                      const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>(x).remainder(y);
}
template <std::size_t P1, std::size_t P2, std::size_t P3>
inline auto fma(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y,
                const teuthid::floatmp<P3> &z) {
  return teuthid::floatmp<std::max(std::max(P1, P2), P3)>(x).fma(y, z);
}
template <std::size_t P1, std::size_t P2>
inline auto fmax(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>(teuthid::floatmp_base::fmax(x, y));
}
template <std::size_t P1, std::size_t P2>
inline auto fmin(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>(teuthid::floatmp_base::fmin(x, y));
}

} // namespace std

#endif // TEUTHID_FLOATMP_HPP