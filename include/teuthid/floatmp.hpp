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
  static_assert((PRECISION <= floatmp_base::max_precision()),                  \
                "Too high floatmp precision.");

class floatmp_base {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  friend class system;
  template <std::size_t Precision> friend class floatmp;
#endif // DOXYGEN_SHOULD_SKIP_THIS

public:
  floatmp_base(std::size_t precision) {
    mpfr_init2(value_, precision);
    mpfr_set_zero(value_, 1);
  }
  floatmp_base(std::size_t precision, const floatmp_base &x) {
    mpfr_init2(value_, precision);
    mpfr_set(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  template <typename T> floatmp_base(std::size_t precision, const T &x) {
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
  const mpfr_t &c_mpfr() const noexcept { return value_; }
  bool is_finite() const { return (mpfr_number_p(value_) != 0); }
  bool is_infinite() const { return (mpfr_inf_p(value_) != 0); }
  bool is_nan() const { return (mpfr_nan_p(value_) != 0); }
  bool is_zero() const { return equal_to(floatmp_base::zero_); }
  bool is_negative() const { return less_than(floatmp_base::zero_); }
  bool is_positive() const { return !is_zero() && !is_negative(); }
  bool is_integer() const;

  static constexpr std::size_t max_precision() noexcept {
    return TEUTHID_FLOAT_MAX_PRECISION;
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
  floatmp_base(std::size_t precision, const TYPE &x) {                         \
    mpfr_init2(value_, precision);                                             \
    FUN(value_, x, static_cast<mpfr_rnd_t>(rounding_mode()));                  \
  }                                                                            \
  void assign(const TYPE &x) {                                                 \
    FUN(value_, x, static_cast<mpfr_rnd_t>(rounding_mode()));                  \
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
  template <std::size_t P> void assign(const floatmp<P> &x) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_set(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }

#define __TEUTHID_FLOATMP_ARITHMETIC_SPEC(OPER, TYPE, FUN)                     \
  void OPER(const TYPE &x) {                                                   \
    FUN(value_, c_mpfr(), x, static_cast<mpfr_rnd_t>(rounding_mode()));        \
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
  void OPER(const long double &x) {                                            \
    mpfr_t __v;                                                                \
    mpfr_init2(__v, mpfr_get_prec(value_));                                    \
    mpfr_set_ld(__v, x, static_cast<mpfr_rnd_t>(rounding_mode()));             \
    FUN(value_, c_mpfr(), __v, static_cast<mpfr_rnd_t>(rounding_mode()));      \
    mpfr_clear(__v);                                                           \
  }                                                                            \
  template <std::size_t P> void OPER(const floatmp<P> &x) {                    \
    TEUTHID_CHECK_FLOATMP_PRECISION(P);                                        \
    FUN(value_, c_mpfr(), x.c_mpfr(),                                          \
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
  floatmp_base(const floatmp_base &x) {
    mpfr_init2(value_, mpfr_get_prec(x.c_mpfr()));
    mpfr_set(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  floatmp_base(floatmp_base &&x) {
    mpfr_init2(value_, mpfr_get_prec(x.c_mpfr()));
    mpfr_swap(value_, x.value_);
  }

  bool equal_to(const floatmp_base &x) const;
  bool less_than(const floatmp_base &x) const;
  void abs(const floatmp_base &x) {
    mpfr_abs(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void fmod(const floatmp_base &x, const floatmp_base &y);
  void remainder(const floatmp_base &x, const floatmp_base &y);
  void fma(const floatmp_base &x, const floatmp_base &y,
           const floatmp_base &z) {
    mpfr_fma(value_, x.c_mpfr(), y.c_mpfr(), z.c_mpfr(),
             static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void fmax(const floatmp_base &x, const floatmp_base &y);
  void fmin(const floatmp_base &x, const floatmp_base &y);
  void fdim(const floatmp_base &x, const floatmp_base &y) {
    mpfr_dim(value_, x.c_mpfr(), y.c_mpfr(),
             static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void exp(const floatmp_base &x) {
    mpfr_exp(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void exp2(const floatmp_base &x) {
    mpfr_exp2(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void expm1(const floatmp_base &x) {
    mpfr_expm1(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void log(const floatmp_base &x);
  void log10(const floatmp_base &x);
  void log2(const floatmp_base &x);
  void log1p(const floatmp_base &x);
  void pow(const floatmp_base &x, const floatmp_base &y);
  void sqrt(const floatmp_base &x);
  void cbrt(const floatmp_base &x) {
    mpfr_cbrt(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void hypot(const floatmp_base &x, const floatmp_base &y) {
    mpfr_hypot(value_, x.c_mpfr(), y.c_mpfr(),
               static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void sin(const floatmp_base &x) {
    mpfr_sin(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void cos(const floatmp_base &x) {
    mpfr_cos(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void tan(const floatmp_base &x) {
    mpfr_tan(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void sinh(const floatmp_base &x) {
    mpfr_sinh(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void cosh(const floatmp_base &x) {
    mpfr_cosh(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void tanh(const floatmp_base &x) {
    mpfr_tanh(value_, x.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void ceil(const floatmp_base &x) { mpfr_ceil(value_, x.c_mpfr()); }
  void floor(const floatmp_base &x) { mpfr_floor(value_, x.c_mpfr()); }
  void trunc(const floatmp_base &x) { mpfr_trunc(value_, x.c_mpfr()); }

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

  mpfr_t value_;
  static std::atomic_int round_mode_;
  static floatmp_base zero_;
  static floatmp_base minus_one_;
  static floatmp_base plus_one_;
}; // class floatmp_base

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef TEUTHID_HAVE_INT_128
template <>
inline floatmp_base::floatmp_base(std::size_t precision, const int128_t &x) {
  mpfr_init2(value_, precision);
  mpfr_set_ld(value_, int128_to_ldouble_(x),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <>
inline floatmp_base::floatmp_base(std::size_t precision, const uint128_t &x) {
  mpfr_init2(value_, precision);
  mpfr_set_ld(value_, uint128_to_ldouble_(x),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <> inline void floatmp_base::assign(const int128_t &x) {
  mpfr_set_ld(value_, int128_to_ldouble_(x),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <> inline void floatmp_base::assign(const uint128_t &x) {
  mpfr_set_ld(value_, uint128_to_ldouble_(x),
              static_cast<mpfr_rnd_t>(rounding_mode()));
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
      mpfr_set(value_, other.c_mpfr(),
               static_cast<mpfr_rnd_t>(rounding_mode()));
    return *this;
  }
  template <std::size_t P> floatmp &operator=(const floatmp<P> &other) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_set(value_, other.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
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
    mpfr_neg(__v.value_, c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <std::size_t P> bool equal_to(const floatmp<P> &x) const {
    return floatmp_base::equal_to(static_cast<const floatmp_base &>(x));
  }
  template <std::size_t P> bool less_than(const floatmp<P> &x) const {
    return floatmp_base::less_than(static_cast<const floatmp_base &>(x));
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS

  static floatmp<Precision> zero() {
    return floatmp<Precision>(floatmp_base::zero_);
  }
  static floatmp<Precision> minus_one() {
    return floatmp<Precision>(floatmp_base::minus_one_);
  }
  static floatmp<Precision> plus_one() {
    return floatmp<Precision>(floatmp_base::plus_one_);
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
template <std::size_t P1, std::size_t P2>
inline void swap(teuthid::floatmp<P1> &x, teuthid::floatmp<P2> &y) {
  x.swap(y);
}
template <std::size_t P> inline auto abs(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().abs(x);
}
template <std::size_t P> inline auto fabs(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().abs(x);
}
template <std::size_t P1, std::size_t P2>
inline auto fmod(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fmod(x, y);
}
template <std::size_t P1, std::size_t P2>
inline auto remainder(const teuthid::floatmp<P1> &x,
                      const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().remainder(x, y);
}
template <std::size_t P1, std::size_t P2, std::size_t P3>
inline auto fma(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y,
                const teuthid::floatmp<P3> &z) {
  return teuthid::floatmp<std::max(std::max(P1, P2), P3)>().fma(x, y, z);
}
template <std::size_t P1, std::size_t P2>
inline auto fmax(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fmax(x, y);
}
template <std::size_t P1, std::size_t P2>
inline auto fmin(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fmin(x, y);
}
template <std::size_t P1, std::size_t P2>
inline auto fdim(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().fdim(x, y);
}
template <std::size_t P> inline auto exp(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().exp(x);
}
template <std::size_t P> inline auto exp2(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().exp2(x);
}
template <std::size_t P> inline auto expm1(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().expm1(x);
}
template <std::size_t P> inline auto log(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log(x);
}
template <std::size_t P> inline auto log10(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log10(x);
}
template <std::size_t P> inline auto log2(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log2(x);
}
template <std::size_t P> inline auto log1p(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().log1p(x);
}
template <std::size_t P1, std::size_t P2>
inline auto pow(const teuthid::floatmp<P1> &x, const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().pow(x, y);
}
template <std::size_t P> inline auto sqrt(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().sqrt(x);
}
template <std::size_t P> inline auto cbrt(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().cbrt(x);
}
template <std::size_t P1, std::size_t P2>
inline auto hypot(const teuthid::floatmp<P1> &x,
                  const teuthid::floatmp<P2> &y) {
  return teuthid::floatmp<std::max(P1, P2)>().hypot(x, y);
}
template <std::size_t P> inline auto sin(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().sin(x);
}
template <std::size_t P> inline auto cos(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().cos(x);
}
template <std::size_t P> inline auto tan(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().tan(x);
}
template <std::size_t P> inline auto sinh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().sinh(x);
}
template <std::size_t P> inline auto cosh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().cosh(x);
}
template <std::size_t P> inline auto tanh(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().tanh(x);
}
template <std::size_t P> inline auto ceil(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().ceil(x);
}
template <std::size_t P> inline auto floor(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().floor(x);
}
template <std::size_t P> inline auto trunc(const teuthid::floatmp<P> &x) {
  return teuthid::floatmp<P>().trunc(x);
}

} // namespace std

#endif // TEUTHID_FLOATMP_HPP