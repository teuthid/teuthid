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

/******************************************************************************/

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
  virtual ~floatmp_base() { mpfr_clear(value_); }
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  floatmp_base(floatmp_base &&) = delete;
  floatmp_base &operator=(const floatmp_base &) = delete;
  floatmp_base &operator=(floatmp_base &&) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS

  template <typename T> void assign(const T &value) {
    TETHID_CHECK_TYPE_SPECIALIZATION(T);
  }
  const mpfr_t &c_mpfr() const noexcept { return value_; }
  bool equal_to(const floatmp_base &value) const;
  bool less_than(const floatmp_base &value) const;

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
  void assign(const int8_t &value) {
    mpfr_set_sj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const int16_t &value) {
    mpfr_set_sj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const int32_t &value) {
    mpfr_set_sj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const int64_t &value) {
    mpfr_set_sj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const uint8_t &value) {
    mpfr_set_uj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const uint16_t &value) {
    mpfr_set_uj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const uint32_t &value) {
    mpfr_set_uj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const uint64_t &value) {
    mpfr_set_uj(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const float &value) {
    mpfr_set_flt(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const double &value) {
    mpfr_set_d(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const long double &value) {
    mpfr_set_ld(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  void assign(const mpfr_t &value) {
    mpfr_set(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  template <std::size_t Precision>
  void assign(const floatmp<Precision> &value) {
    mpfr_set(value_, value.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS

private:
  floatmp_base() { mpfr_init2(value_, mpfr_get_default_prec()); }
  floatmp_base(const floatmp_base &value) {
    mpfr_init2(value_, mpfr_get_prec(value.value_));
    mpfr_set(value_, value.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
  }
  mpfr_t value_;
  static std::atomic_int round_mode_;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef TEUTHID_HAVE_INT_128
template <> void floatmp_base::assign(const int128_t &value);
template <> void floatmp_base::assign(const uint128_t &value);
#endif // TEUTHID_HAVE_INT_128
#endif // DOXYGEN_SHOULD_SKIP_THIS

/******************************************************************************/

inline bool operator==(const floatmp_base &lhs, const floatmp_base &rhs) {
  return lhs.equal_to(rhs);
}
inline bool operator!=(const floatmp_base &lhs, const floatmp_base &rhs) {
  return !(lhs == rhs);
}
inline bool operator<(const floatmp_base &lhs, const floatmp_base &rhs) {
  return lhs.less_than(rhs);
}
inline bool operator>(const floatmp_base &lhs, const floatmp_base &rhs) {
  return rhs < lhs;
}
inline bool operator<=(const floatmp_base &lhs, const floatmp_base &rhs) {
  return !(lhs > rhs);
}
inline bool operator>=(const floatmp_base &lhs, const floatmp_base &rhs) {
  return !(lhs < rhs);
}

/******************************************************************************/

#define TEUTHID_CHECK_FLOATMP_PRECISION(PRECISION)                             \
  static_assert((PRECISION >= floatmp_base::min_precision()),                  \
                "Too low floatmp precision.");                                 \
  static_assert((PRECISION < floatmp_base::max_precision()),                   \
                "Too high floatmp precision.");

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
  virtual ~floatmp() {}
  floatmp &operator=(const floatmp &other) {
    if (this != &other)
      mpfr_set(value_, other.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
    return *this;
  }
  template <std::size_t P> floatmp &operator=(const floatmp<P> &other) {
    mpfr_set(value_, other.value_, static_cast<mpfr_rnd_t>(rounding_mode()));
    return *this;
  }
  template <typename T> floatmp &operator=(const T &value) {
    floatmp_base::assign(value);
  }
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <std::size_t P> floatmp(floatmp<P> &&value) = delete;
  template <std::size_t P> floatmp &operator=(floatmp<P> &&other) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS

  template <typename T> void assign(const T &value) {
    floatmp_base::assign(value);
  }
  constexpr std::size_t precision() const noexcept { return Precision; }
};

/******************************************************************************/

template <std::size_t P1, std::size_t P2>
inline bool operator==(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return lhs.equal_to(static_cast<const floatmp_base &>(rhs));
}
template <std::size_t P1, std::size_t P2>
inline bool operator!=(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return !(lhs == rhs);
}
template <std::size_t P1, std::size_t P2>
inline bool operator<(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return lhs.less_than(static_cast<const floatmp_base &>(rhs));
}
template <std::size_t P1, std::size_t P2>
inline bool operator>(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return rhs < lhs;
}
template <std::size_t P1, std::size_t P2>
inline bool operator<=(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return !(lhs > rhs);
}
template <std::size_t P1, std::size_t P2>
inline bool operator>=(const floatmp<P1> &lhs, const floatmp<P2> &rhs) {
  return !(lhs < rhs);
}

} // namespace teuthid

#endif // TEUTHID_FLOATMP_HPP