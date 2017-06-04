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

template <std::size_t Precision> class floatmp;

class floatmp_base {
  template <std::size_t Precision> friend class floatmp;

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

  template <typename T> void add(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  template <typename T> void assign(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  const mpfr_t &c_mpfr() const noexcept { return value_; }

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

#define __TEUTHID_FLOATMP_ADD_SPEC(TYPE, FUN)                                  \
  void add(const TYPE &value) {                                                \
    FUN(value_, c_mpfr(), value, static_cast<mpfr_rnd_t>(rounding_mode()));    \
  }
  __TEUTHID_FLOATMP_ADD_SPEC(int8_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ADD_SPEC(int16_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ADD_SPEC(int32_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ADD_SPEC(int64_t, mpfr_add_si)
  __TEUTHID_FLOATMP_ADD_SPEC(uint8_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ADD_SPEC(uint16_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ADD_SPEC(uint32_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ADD_SPEC(uint64_t, mpfr_add_ui)
  __TEUTHID_FLOATMP_ADD_SPEC(float, mpfr_add_d)
  __TEUTHID_FLOATMP_ADD_SPEC(double, mpfr_add_d)
  __TEUTHID_FLOATMP_ADD_SPEC(mpfr_t, mpfr_add)
#undef __TEUTHID_FLOATMP_ADD_SPEC
  void add(const long double &value) {
    mpfr_t __v;
    mpfr_init2(__v, mpfr_get_prec(value_));
    mpfr_set_ld(__v, value, static_cast<mpfr_rnd_t>(rounding_mode()));
    mpfr_add(value_, c_mpfr(), __v, static_cast<mpfr_rnd_t>(rounding_mode()));
    mpfr_clear(__v);
  }
  template <std::size_t P> void add(const floatmp<P> &value) {
    TEUTHID_CHECK_FLOATMP_PRECISION(P);
    mpfr_add(value_, c_mpfr(), value.value_,
             static_cast<mpfr_rnd_t>(rounding_mode()));
  }
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
};

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
template <> inline void floatmp_base::add(const int128_t &value) {
  add(int128_to_ldouble_(value));
}
template <> inline void floatmp_base::add(const uint128_t &value) {
  add(uint128_to_ldouble_(value));
}
template <> inline void floatmp_base::assign(const int128_t &value) {
  mpfr_set_ld(value_, int128_to_ldouble_(value),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
template <> inline void floatmp_base::assign(const uint128_t &value) {
  mpfr_set_ld(value_, uint128_to_ldouble_(value),
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
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
  template <typename T> floatmp &operator+=(const T &value) {
    floatmp_base::add(value);
    return *this;
  }

  template <typename T> floatmp &add(const T &value) {
    floatmp_base::add(value);
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
  constexpr std::size_t precision() const noexcept { return Precision; }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <std::size_t P> bool equal_to(const floatmp<P> &value) const {
    return floatmp_base::equal_to(static_cast<const floatmp_base &>(value));
  }
  template <std::size_t P> bool less_than(const floatmp<P> &value) const {
    return floatmp_base::less_than(static_cast<const floatmp_base &>(value));
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS
};

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

// operator +
template <std::size_t P1, std::size_t P2>
inline const floatmp<std::max(P1, P2)> operator+(const floatmp<P1> &lhs,
                                                 const floatmp<P2> &rhs) {
  return floatmp<std::max(P1, P2)>(lhs).add(rhs);
}

} // namespace teuthid

#endif // TEUTHID_FLOATMP_HPP