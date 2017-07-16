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
#include <cassert>
#include <cmath>
#include <limits>

#include <mpfr.h>
#include <teuthid/config.hpp>

// forward declarations:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace teuthid {
class system;
template <std::size_t Precision> class floatmp;
}
#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace teuthid {

enum class floatmp_round_t : int {
  round_to_nearest = MPFR_RNDN,
  round_toward_zero = MPFR_RNDZ,
  round_toward_infinity = MPFR_RNDU,
  round_toward_neg_infinity = MPFR_RNDD,
  round_away_from_zero = MPFR_RNDA,
  round_to_nearest_away_from_zero = MPFR_RNDNA
};

#define TEUTHID_ASSERT_FLOATMP_PRECISION(PRECISION)                            \
  assert(PRECISION >= floatmp_base::min_precision());                          \
  assert(PRECISION <= floatmp_base::max_precision());

#define TEUTHID_CHECK_FLOATMP_PRECISION(PRECISION)                             \
  static_assert((PRECISION >= floatmp_base::min_precision()),                  \
                "Too low floatmp precision.");                                 \
  static_assert((PRECISION <= floatmp_base::max_precision()),                  \
                "Too high floatmp precision.");

class floatmp_base {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  friend class system;
  template <std::size_t Precision> friend class floatmp;
#endif // DOXYGEN_SHOULD_SKIP_THIS

public:
  floatmp_base(std::size_t precision) {
    TEUTHID_ASSERT_FLOATMP_PRECISION(precision);
    mpfr_init2(value_, precision);
    mpfr_set_zero(value_, 1);
  }
  floatmp_base(std::size_t precision, const floatmp_base &x) {
    TEUTHID_ASSERT_FLOATMP_PRECISION(precision);
    mpfr_init2(value_, precision);
    mpfr_set(value_, x.c_mpfr(), mpfr_rnd_());
  }
  template <typename T> floatmp_base(std::size_t precision, const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  virtual ~floatmp_base() { mpfr_clear(value_); }
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  floatmp_base &operator=(const floatmp_base &) = delete;
  floatmp_base &operator=(floatmp_base &&) = delete;

#define __TEUTHID_FLOATMP_CTOR_SPEC(TYPE, FUN)                                 \
  floatmp_base(std::size_t precision, const TYPE &x) {                         \
    TEUTHID_ASSERT_FLOATMP_PRECISION(precision);                               \
    mpfr_init2(value_, precision);                                             \
    FUN(value_, x, mpfr_rnd_());                                               \
  }
  __TEUTHID_FLOATMP_CTOR_SPEC(int8_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_CTOR_SPEC(int16_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_CTOR_SPEC(int32_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_CTOR_SPEC(int64_t, mpfr_set_sj)
  __TEUTHID_FLOATMP_CTOR_SPEC(uint8_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_CTOR_SPEC(uint16_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_CTOR_SPEC(uint32_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_CTOR_SPEC(uint64_t, mpfr_set_uj)
  __TEUTHID_FLOATMP_CTOR_SPEC(float, mpfr_set_flt)
  __TEUTHID_FLOATMP_CTOR_SPEC(double, mpfr_set_d)
  __TEUTHID_FLOATMP_CTOR_SPEC(long double, mpfr_set_ld)
  __TEUTHID_FLOATMP_CTOR_SPEC(mpfr_t, mpfr_set)
#undef __TEUTHID_FLOATMP_CTOR_SPEC
#endif // DOXYGEN_SHOULD_SKIP_THIS

  explicit operator float() const { return mpfr_get_flt(value_, mpfr_rnd_()); }
  explicit operator double() const { return mpfr_get_d(value_, mpfr_rnd_()); }
  explicit operator long double() const {
    return mpfr_get_ld(value_, mpfr_rnd_());
  }

  const mpfr_t &c_mpfr() const noexcept { return value_; }
  bool is_finite() const { return (mpfr_number_p(value_) != 0); }
  bool is_infinite() const { return (mpfr_inf_p(value_) != 0); }
  bool is_nan() const { return (mpfr_nan_p(value_) != 0); }
  bool is_zero() const { return equal_to(zero_); }
  bool is_negative() const { return less_than(zero_); }
  bool is_positive() const { return !is_zero() && !is_negative(); }
  bool is_integer() const;

  static constexpr std::size_t max_precision() noexcept {
    return TEUTHID_FLOAT_MAX_PRECISION;
  }
  static constexpr std::size_t min_precision() noexcept {
    return MPFR_PREC_MIN;
  }
  static floatmp_round_t rounding_mode() noexcept {
    return static_cast<floatmp_round_t>(round_mode_.load());
  }
  static floatmp_round_t rounding_mode(floatmp_round_t mode) noexcept {
    return static_cast<floatmp_round_t>(
        round_mode_.exchange(static_cast<int>(mode)));
  }

private:
  floatmp_base() { mpfr_init2(value_, mpfr_get_default_prec()); }
  floatmp_base(const floatmp_base &x) {
    mpfr_init2(value_, mpfr_get_prec(x.c_mpfr()));
    mpfr_set(value_, x.c_mpfr(), mpfr_rnd_());
  }
  floatmp_base(floatmp_base &&x) {
    mpfr_init2(value_, mpfr_get_prec(x.c_mpfr()));
    mpfr_swap(value_, x.value_);
  }

  template <typename T> void assign(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void add(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void sub(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void mul(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void div(const T &x) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  bool equal_to(const floatmp_base &x) const;
  bool less_than(const floatmp_base &x) const;
  void abs(const floatmp_base &x) { mpfr_abs(value_, x.c_mpfr(), mpfr_rnd_()); }
  void fmod(const floatmp_base &x, const floatmp_base &y);
  void remainder(const floatmp_base &x, const floatmp_base &y);
  void fma(const floatmp_base &x, const floatmp_base &y,
           const floatmp_base &z) {
    mpfr_fma(value_, x.c_mpfr(), y.c_mpfr(), z.c_mpfr(), mpfr_rnd_());
  }
  void fmax(const floatmp_base &x, const floatmp_base &y);
  void fmin(const floatmp_base &x, const floatmp_base &y);
  void fdim(const floatmp_base &x, const floatmp_base &y) {
    mpfr_dim(value_, x.c_mpfr(), y.c_mpfr(), mpfr_rnd_());
  }
  void exp(const floatmp_base &x) { mpfr_exp(value_, x.c_mpfr(), mpfr_rnd_()); }
  void exp2(const floatmp_base &x) {
    mpfr_exp2(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void expm1(const floatmp_base &x) {
    mpfr_expm1(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void log(const floatmp_base &x);
  void log10(const floatmp_base &x);
  void log2(const floatmp_base &x);
  void log1p(const floatmp_base &x);
  void pow(const floatmp_base &x, const floatmp_base &y);
  void sqrt(const floatmp_base &x);
  void cbrt(const floatmp_base &x) {
    mpfr_cbrt(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void hypot(const floatmp_base &x, const floatmp_base &y) {
    mpfr_hypot(value_, x.c_mpfr(), y.c_mpfr(), mpfr_rnd_());
  }
  void sin(const floatmp_base &x) { mpfr_sin(value_, x.c_mpfr(), mpfr_rnd_()); }
  void cos(const floatmp_base &x) { mpfr_cos(value_, x.c_mpfr(), mpfr_rnd_()); }
  void tan(const floatmp_base &x) { mpfr_tan(value_, x.c_mpfr(), mpfr_rnd_()); }
  void asin(const floatmp_base &x);
  void acos(const floatmp_base &x);
  void atan(const floatmp_base &x) {
    mpfr_atan(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void atan2(const floatmp_base &x, const floatmp_base &y) {
    mpfr_atan2(value_, x.c_mpfr(), y.c_mpfr(), mpfr_rnd_());
  }
  void sinh(const floatmp_base &x) {
    mpfr_sinh(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void cosh(const floatmp_base &x) {
    mpfr_cosh(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void tanh(const floatmp_base &x) {
    mpfr_tanh(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void asinh(const floatmp_base &x) {
    mpfr_asinh(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void acosh(const floatmp_base &x);
  void atanh(const floatmp_base &x);
  void erf(const floatmp_base &x) { mpfr_erf(value_, x.c_mpfr(), mpfr_rnd_()); }
  void erfc(const floatmp_base &x) {
    mpfr_erfc(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void tgamma(const floatmp_base &x);
  void lgamma(const floatmp_base &x);
  void ceil(const floatmp_base &x) { mpfr_ceil(value_, x.c_mpfr()); }
  void floor(const floatmp_base &x) { mpfr_floor(value_, x.c_mpfr()); }
  void trunc(const floatmp_base &x) { mpfr_trunc(value_, x.c_mpfr()); }
  void round(const floatmp_base &x) { mpfr_round(value_, x.c_mpfr()); }
  void nearbyint(const floatmp_base &x) {
    mpfr_rint(value_, x.c_mpfr(), mpfr_rnd_());
  }
  void nextafter(const floatmp_base &x, const floatmp_base &y);
  void nextabove(const floatmp_base &x);
  void nextbelow(const floatmp_base &x);
#ifdef TEUTHID_HAVE_INT_128
  static long double int128_to_ldouble_(const int128_t &x) {
    return static_cast<long double>(INT64_MAX) *
               static_cast<long double>(x / INT64_MAX) +
           static_cast<long double>(x % INT64_MAX);
  }
  static long double uint128_to_ldouble_(const uint128_t &x) {
    return static_cast<long double>(UINT64_MAX) *
               static_cast<long double>(x / UINT64_MAX) +
           static_cast<long double>(x % UINT64_MAX);
  }
#endif // TEUTHID_HAVE_INT_128

#define __TEUTHID_FLOATMP_ASSIGN_SPEC(TYPE, FUN)                               \
  void assign(const TYPE &x) { FUN(value_, x, mpfr_rnd_()); }
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
  template <std::size_t P> void assign(const floatmp<P> &x) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_set(value_, x.c_mpfr(), mpfr_rnd_());
  }

#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER, TYPE, FUN)                     \
  void OPER(const TYPE &x) { FUN(value_, c_mpfr(), x, mpfr_rnd_()); }
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
  void OPER(const long double &x) {                                            \
    mpfr_t __v;                                                                \
    mpfr_init2(__v, mpfr_get_prec(value_));                                    \
    mpfr_set_ld(__v, x, mpfr_rnd_());                                          \
    FUN(value_, c_mpfr(), __v, mpfr_rnd_());                                   \
    mpfr_clear(__v);                                                           \
  }                                                                            \
  template <std::size_t P> void OPER(const floatmp<P> &x) {                    \
    TEUTHID_CHECK_FLOATMP_PRECISION(P);                                        \
    FUN(value_, c_mpfr(), x.c_mpfr(), mpfr_rnd_());                            \
  }
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(add, mpfr_add)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(sub, mpfr_sub)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(mul, mpfr_mul)
  __TEUTHID_FLOATMP_ARITHMETIC_SPEC(div, mpfr_div)
#undef __TEUTHID_FLOATMP_ARITHMETIC_SPEC

  static mpfr_rnd_t mpfr_rnd_() noexcept {
    return static_cast<mpfr_rnd_t>(round_mode_.load());
  }

  mpfr_t value_;
  static std::atomic_int round_mode_;
  static const floatmp_base zero_;
  static const floatmp_base minus_one_;
  static const floatmp_base plus_one_;
}; // class floatmp_base

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef TEUTHID_HAVE_INT_128
template <>
inline floatmp_base::floatmp_base(std::size_t precision, const int128_t &x) {
  TEUTHID_ASSERT_FLOATMP_PRECISION(precision);
  mpfr_init2(value_, precision);
  mpfr_set_ld(value_, int128_to_ldouble_(x), mpfr_rnd_());
}
template <>
inline floatmp_base::floatmp_base(std::size_t precision, const uint128_t &x) {
  TEUTHID_ASSERT_FLOATMP_PRECISION(precision);
  mpfr_init2(value_, precision);
  mpfr_set_ld(value_, uint128_to_ldouble_(x), mpfr_rnd_());
}
template <> inline void floatmp_base::assign(const int128_t &x) {
  mpfr_set_ld(value_, int128_to_ldouble_(x), mpfr_rnd_());
}
template <> inline void floatmp_base::assign(const uint128_t &x) {
  mpfr_set_ld(value_, uint128_to_ldouble_(x), mpfr_rnd_());
}

#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER)                                \
  template <> inline void floatmp_base::OPER(const int128_t &x) {              \
    OPER(int128_to_ldouble_(x));                                               \
  }                                                                            \
  template <> inline void floatmp_base::OPER(const uint128_t &x) {             \
    OPER(uint128_to_ldouble_(x));                                              \
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
  floatmp(const floatmp<P> &x)
      : floatmp_base(Precision, static_cast<const floatmp_base &>(x)) {
    TEUTHID_CHECK_FLOATMP_PRECISION(Precision);
  }
  template <typename T> floatmp(const T &x) : floatmp_base(Precision, x) {
    TEUTHID_CHECK_FLOATMP_PRECISION(Precision);
  }
  virtual ~floatmp() {}
  floatmp &operator=(const floatmp &other) {
    if (this != &other)
      mpfr_set(value_, other.c_mpfr(), mpfr_rnd_());
    return *this;
  }
  template <std::size_t P> floatmp &operator=(const floatmp<P> &other) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_set(value_, other.c_mpfr(), mpfr_rnd_());
    return *this;
  }
  template <typename T> floatmp &operator=(const T &x) {
    floatmp_base::assign(x);
    return *this;
  }
  template <std::size_t P> operator floatmp<P>() const {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    return floatmp<P>(*this);
  }
  floatmp operator-() const {
    floatmp<Precision> __v(*this);
    mpfr_neg(__v.value_, c_mpfr(), mpfr_rnd_());
    return __v;
  }
  template <typename T> floatmp &operator+=(const T &x) {
    floatmp_base::add(x);
    return *this;
  }
  template <typename T> floatmp &operator-=(const T &x) {
    floatmp_base::sub(x);
    return *this;
  }
  template <typename T> floatmp &operator*=(const T &x) {
    floatmp_base::mul(x);
    return *this;
  }
  template <typename T> floatmp &operator/=(const T &x) {
    floatmp_base::div(x);
    return *this;
  }

  template <typename T> floatmp &assign(const T &x) {
    floatmp_base::assign(x);
    return *this;
  }
  template <typename T> bool equal_to(const T &x) const {
    return floatmp_base::equal_to(
        static_cast<floatmp_base>(floatmp<Precision>(x)));
  }
  template <typename T> bool less_than(const T &x) const {
    return floatmp_base::less_than(
        static_cast<floatmp_base>(floatmp<Precision>(x)));
  }
  template <typename T> floatmp &add(const T &x) {
    floatmp_base::add(x);
    return *this;
  }
  template <typename T> floatmp &sub(const T &x) {
    floatmp_base::sub(x);
    return *this;
  }
  template <typename T> floatmp &mul(const T &x) {
    floatmp_base::mul(x);
    return *this;
  }
  template <typename T> floatmp &div(const T &x) {
    floatmp_base::div(x);
    return *this;
  }
  constexpr std::size_t precision() const noexcept { return Precision; }
  template <std::size_t P> floatmp &swap(floatmp<P> &x) {
    floatmp __tmp(x);
    x.assign(c_mpfr());
    return assign(__tmp);
  }
  template <std::size_t P> floatmp &abs(const floatmp<P> &x) {
    floatmp_base::abs(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &fmod(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::fmod(static_cast<const floatmp_base &>(x),
                       static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &remainder(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::remainder(static_cast<const floatmp_base &>(x),
                            static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P1, std::size_t P2, std::size_t P3>
  floatmp &fma(const floatmp<P1> &x, const floatmp<P2> &y,
               const floatmp<P3> &z) {
    floatmp_base::fma(static_cast<const floatmp_base &>(x),
                      static_cast<const floatmp_base &>(y),
                      static_cast<const floatmp_base &>(z));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &fmax(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::fmax(static_cast<const floatmp_base &>(x),
                       static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &fmin(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::fmin(static_cast<const floatmp_base &>(x),
                       static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &fdim(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::fdim(static_cast<const floatmp_base &>(x),
                       static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P> floatmp &exp(const floatmp<P> &x) {
    floatmp_base::exp(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &exp2(const floatmp<P> &x) {
    floatmp_base::exp2(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &expm1(const floatmp<P> &x) {
    floatmp_base::expm1(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &log(const floatmp<P> &x) {
    floatmp_base::log(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &log10(const floatmp<P> &x) {
    floatmp_base::log10(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &log2(const floatmp<P> &x) {
    floatmp_base::log2(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &log1p(const floatmp<P> &x) {
    floatmp_base::log1p(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &pow(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::pow(static_cast<const floatmp_base &>(x),
                      static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P> floatmp &sqrt(const floatmp<P> &x) {
    floatmp_base::sqrt(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &cbrt(const floatmp<P> &x) {
    floatmp_base::cbrt(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &hypot(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::hypot(static_cast<const floatmp_base &>(x),
                        static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P> floatmp &sin(const floatmp<P> &x) {
    floatmp_base::sin(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &cos(const floatmp<P> &x) {
    floatmp_base::cos(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &tan(const floatmp<P> &x) {
    floatmp_base::tan(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &asin(const floatmp<P> &x) {
    floatmp_base::asin(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &acos(const floatmp<P> &x) {
    floatmp_base::acos(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &atan(const floatmp<P> &x) {
    floatmp_base::atan(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &atan2(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::atan2(static_cast<const floatmp_base &>(x),
                        static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P> floatmp &sinh(const floatmp<P> &x) {
    floatmp_base::sinh(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &cosh(const floatmp<P> &x) {
    floatmp_base::cosh(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &tanh(const floatmp<P> &x) {
    floatmp_base::tanh(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &asinh(const floatmp<P> &x) {
    floatmp_base::asinh(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &acosh(const floatmp<P> &x) {
    floatmp_base::acosh(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &atanh(const floatmp<P> &x) {
    floatmp_base::atanh(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &erf(const floatmp<P> &x) {
    floatmp_base::erf(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &erfc(const floatmp<P> &x) {
    floatmp_base::erfc(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &tgamma(const floatmp<P> &x) {
    floatmp_base::tgamma(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &lgamma(const floatmp<P> &x) {
    floatmp_base::lgamma(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &ceil(const floatmp<P> &x) {
    floatmp_base::ceil(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &floor(const floatmp<P> &x) {
    floatmp_base::floor(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &trunc(const floatmp<P> &x) {
    floatmp_base::trunc(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &round(const floatmp<P> &x) {
    floatmp_base::round(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &nearbyint(const floatmp<P> &x) {
    floatmp_base::nearbyint(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P1, std::size_t P2>
  floatmp &nextafter(const floatmp<P1> &x, const floatmp<P2> &y) {
    floatmp_base::nextafter(static_cast<const floatmp_base &>(x),
                            static_cast<const floatmp_base &>(y));
    return *this;
  }
  template <std::size_t P> floatmp &nextabove(const floatmp<P> &x) {
    floatmp_base::nextabove(static_cast<const floatmp_base &>(x));
    return *this;
  }
  template <std::size_t P> floatmp &nextbelow(const floatmp<P> &x) {
    floatmp_base::nextbelow(static_cast<const floatmp_base &>(x));
    return *this;
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <std::size_t P> bool equal_to(const floatmp<P> &x) const {
    return floatmp_base::equal_to(static_cast<const floatmp_base &>(x));
  }
  template <std::size_t P> bool less_than(const floatmp<P> &x) const {
    return floatmp_base::less_than(static_cast<const floatmp_base &>(x));
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS

  inline static constexpr floatmp<Precision> zero() {
    return floatmp<Precision>(floatmp_base::zero_);
  }
  inline static constexpr floatmp<Precision> minus_one() {
    return floatmp<Precision>(floatmp_base::minus_one_);
  }
  inline static constexpr floatmp<Precision> plus_one() {
    return floatmp<Precision>(floatmp_base::plus_one_);
  }
  inline static constexpr floatmp<Precision> epsilon() {
    return floatmp<Precision>().nextabove(plus_one()) - plus_one();
  }
  inline static constexpr floatmp<Precision> min() {
    return floatmp<Precision>().exp2(floatmp<Precision>(mpfr_get_emin() - 1));
  }
  inline static constexpr floatmp<Precision> max() {
    return (plus_one() - epsilon()) *
           floatmp<Precision>().exp2(floatmp<Precision>(mpfr_get_emax()));
  }
  inline static constexpr floatmp<Precision> round_error() {
    if (rounding_mode() == floatmp_round_t::round_to_nearest)
      return floatmp<Precision>(0.5);
    else
      return floatmp<Precision>(1.0);
  }
  static constexpr floatmp<Precision> infinity() {
    floatmp<Precision> __x;
    mpfr_set_inf(__x.value_, 1);
    return __x;
  }
  static constexpr floatmp<Precision> nan() {
    floatmp<Precision> __x;
    mpfr_set_nan(__x.value_);
    return __x;
  }
}; // class floatmp

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
template <size_t P1, size_t P2>
inline void swap(teuthid::floatmp<P1> &x, teuthid::floatmp<P2> &y) {
  x.swap(y);
}
template <size_t P> inline auto abs(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().abs(x);
}
template <size_t P> inline auto fabs(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().abs(x);
}
template <size_t P1, size_t P2>
inline auto fmod(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fmod(x, y);
}
template <size_t P1, size_t P2>
inline auto remainder(const teuthid::floatmp<P1> &x,
                      const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().remainder(x, y);
}
template <size_t P1, size_t P2, size_t P3>
inline auto fma(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y,
                const teuthid::floatmp<P3> &z) {
  return teuthid::floatmp<std::max(std::max(P1, P2), P3)>().fma(x, y, z);
}
template <size_t P1, size_t P2>
inline auto fmax(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fmax(x, y);
}
template <size_t P1, size_t P2>
inline auto fmin(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fmin(x, y);
}
template <size_t P1, size_t P2>
inline auto fdim(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fdim(x, y);
}
template <size_t P> inline auto exp(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().exp(x);
}
template <size_t P> inline auto exp2(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().exp2(x);
}
template <size_t P> inline auto expm1(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().expm1(x);
}
template <size_t P> inline auto log(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log(x);
}
template <size_t P> inline auto log10(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log10(x);
}
template <size_t P> inline auto log2(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log2(x);
}
template <size_t P> inline auto log1p(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log1p(x);
}
template <size_t P1, std::size_t P2>
inline auto pow(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().pow(x, y);
}
template <size_t P> inline auto sqrt(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().sqrt(x);
}
template <size_t P> inline auto cbrt(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().cbrt(x);
}
template <size_t P1, size_t P2>
inline auto hypot(const teuthid::floatmp<P1> &x,
                  const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().hypot(x, y);
}
template <size_t P> inline auto sin(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().sin(x);
}
template <size_t P> inline auto cos(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().cos(x);
}
template <size_t P> inline auto tan(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().tan(x);
}
template <size_t P> inline auto asin(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().asin(x);
}
template <size_t P> inline auto acos(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().acos(x);
}
template <size_t P> inline auto atan(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().atan(x);
}
template <size_t P1, size_t P2>
inline auto atan2(const teuthid::floatmp<P1> &x,
                  const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().atan2(x, y);
}
template <size_t P> inline auto sinh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().sinh(x);
}
template <size_t P> inline auto cosh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().cosh(x);
}
template <size_t P> inline auto tanh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().tanh(x);
}
template <size_t P> inline auto asinh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().asinh(x);
}
template <size_t P> inline auto acosh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().acosh(x);
}
template <size_t P> inline auto atanh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().atanh(x);
}
template <size_t P> inline auto erf(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().erf(x);
}
template <size_t P> inline auto erfc(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().erfc(x);
}
template <size_t P> inline auto tgamma(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().tgamma(x);
}
template <size_t P> inline auto lgamma(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().lgamma(x);
}
template <size_t P> inline auto ceil(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().ceil(x);
}
template <size_t P> inline auto floor(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().floor(x);
}
template <size_t P> inline auto trunc(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().trunc(x);
}
template <size_t P> inline auto round(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().round(x);
}
template <size_t P> inline long lround(const teuthid::floatmp<P> &x) {
  return mpfr_get_si(x.c_mpfr(), static_cast<mpfr_rnd_t>(
                                     teuthid::floatmp_base::rounding_mode()));
}
template <size_t P> inline long long llround(const teuthid::floatmp<P> &x) {
  return mpfr_get_sj(x.c_mpfr(), static_cast<mpfr_rnd_t>(
                                     teuthid::floatmp_base::rounding_mode()));
}
template <size_t P> inline auto nearbyint(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().nearbyint(x);
}
template <size_t P> inline auto rint(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().nearbyint(x);
}
template <size_t P1, size_t P2>
inline auto nextafter(const teuthid::floatmp<P1> &x,
                      const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().nextafter(x, y);
}
template <size_t P>
inline auto nexttoward(const teuthid::floatmp<P> &x, long double y) {
  return teuthid::floatmp<P>().nextafter(x, teuthid::floatmp<P>(y));
}
template <size_t P> inline bool isfinite(const teuthid::floatmp<P> &x) {
  return x.is_finite();
}
template <size_t P> inline bool isinf(const teuthid::floatmp<P> &x) {
  return x.is_infinite();
}
template <size_t P> inline bool isnan(const teuthid::floatmp<P> &x) {
  return x.is_nan();
}
template <size_t P> inline bool isnormal(const teuthid::floatmp<P> &x) {
  return x.is_finite() && !x.is_zero();
}
template <size_t P> inline bool signbit(const teuthid::floatmp<P> &x) {
  return x.is_negative();
}
template <size_t P1, size_t P2>
inline bool isgreater(const teuthid::floatmp<P1> &x,
                      const teuthid::floatmp<P2> &y) {
  return x > y;
}
template <size_t P1, size_t P2>
inline bool isgreaterequal(const teuthid::floatmp<P1> &x,
                           const teuthid::floatmp<P2> &y) {
  return x >= y;
}
template <size_t P1, size_t P2>
inline bool isless(const teuthid::floatmp<P1> &x,
                   const teuthid::floatmp<P2> &y) {
  return x < y;
}
template <size_t P1, size_t P2>
inline bool islessequal(const teuthid::floatmp<P1> &x,
                        const teuthid::floatmp<P2> &y) {
  return x <= y;
}
template <size_t P1, size_t P2>
inline bool islessgreater(const teuthid::floatmp<P1> &x,
                          const teuthid::floatmp<P2> &y) {
  return (x < y) || (y < x);
}
template <size_t P1, size_t P2>
inline bool isunordered(const teuthid::floatmp<P1> &x,
                        const teuthid::floatmp<P2> &y) {
  return x.is_nan() || y.is_nan();
}

template <> template <size_t P> class numeric_limits<teuthid::floatmp<P>> {
private:
  static constexpr float_round_style round_style_() noexcept;

public:
  static constexpr bool is_specialized = true;
  static constexpr teuthid::floatmp<P> min() noexcept {
    return teuthid::floatmp<P>::min();
  }
  static constexpr teuthid::floatmp<P> max() noexcept {
    return teuthid::floatmp<P>::max();
  }
  static constexpr teuthid::floatmp<P> lowest() noexcept {
    return -(teuthid::floatmp<P>::max());
  }
  static constexpr int digits = P;
  static constexpr int digits10 = floor(log10(2) * P);
  static constexpr int max_digits10 = digits10 + 1;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = false;
  static constexpr bool is_exact = false;
  static constexpr int radix = 2;
  static constexpr teuthid::floatmp<P> epsilon() noexcept {
    return teuthid::floatmp<P>::epsilon();
  }
  static constexpr teuthid::floatmp<P> round_error() noexcept {
    return teuthid::floatmp<P>::round_error();
  }
  static constexpr int min_exponent = MPFR_EMIN_DEFAULT;
  static constexpr int min_exponent10 = floor(log10(2) * MPFR_EMIN_DEFAULT);
  static constexpr int max_exponent = MPFR_EMAX_DEFAULT;
  static constexpr int max_exponent10 = floor(log10(2) * MPFR_EMAX_DEFAULT);
  static constexpr bool has_infinity = true;
  static constexpr bool has_quiet_NaN = true;
  static constexpr bool has_signaling_NaN = true;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr bool has_denorm_loss = false;
  static constexpr teuthid::floatmp<P> infinity() noexcept {
    return teuthid::floatmp<P>::infinity();
  }
  static constexpr teuthid::floatmp<P> quiet_NaN() noexcept {
    return teuthid::floatmp<P>::nan();
  }
  static constexpr teuthid::floatmp<P> signaling_NaN() noexcept {
    return teuthid::floatmp<P>::nan();
  }
  static constexpr teuthid::floatmp<P> denorm_min() noexcept {
    return teuthid::floatmp<P>::min();
  }
  static constexpr bool is_iec559 = true;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = false;
  static constexpr bool traps = true;
  static constexpr bool tinyness_before = true;
  static constexpr float_round_style round_style = round_style_();
}; // class numeric_limits<teuthid::floatmp<P>>

template <>
template <size_t P>
constexpr float_round_style
numeric_limits<teuthid::floatmp<P>>::round_style_() noexcept {
  switch (teuthid::floatmp_base::rounding_mode()) {
  case teuthid::floatmp_round_t::round_to_nearest:
    return round_to_nearest;
  case teuthid::floatmp_round_t::round_toward_zero:
    return round_toward_zero;
  case teuthid::floatmp_round_t::round_toward_infinity:
    return round_toward_infinity;
  case teuthid::floatmp_round_t::round_toward_neg_infinity:
    return round_toward_neg_infinity;
  case teuthid::floatmp_round_t::round_toward_neg_infinity:
    return round_toward_neg_infinity;
  default:
    return round_indeterminate;
  }
}
} // namespace std

#endif // TEUTHID_FLOATMP_HPP