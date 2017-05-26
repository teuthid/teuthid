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

#include <mutex>

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

class floatmp_base {
public:
  floatmp_base(std::size_t precision);
  floatmp_base(std::size_t precision, const floatmp_base &value);
  virtual ~floatmp_base();
#ifndef DOXYGEN_SHOULD_SKIP_THIS  
  floatmp_base(const floatmp_base &) = delete;
  floatmp_base(floatmp_base &&) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS
  // floatmp_base &operator=(const floatmp_base &) = default;
  // floatmp_base &operator=(floatmp_base &&) = default;

  const mpfr_t &c_mpfr() const noexcept { return value_; }
  static constexpr std::size_t max_precision() noexcept {
    return MPFR_PREC_MAX;
  }
  static constexpr std::size_t min_precision() noexcept {
    return MPFR_PREC_MIN;
  }
  static floatmp_round_t rounding_mode();
  static floatmp_round_t rounding_mode(floatmp_round_t mode);

private:
  floatmp_base() {}
  mpfr_t value_;
  static mpfr_rnd_t round_mode_;
  static std::mutex round_mode_mutex_;
};

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
  floatmp(const floatmp &value)
      : floatmp_base(Precision, static_cast<floatmp_base>(value)) {
    TEUTHID_CHECK_FLOATMP_PRECISION(Precision);
  }
  virtual ~floatmp() {}

  constexpr std::size_t precision() const noexcept { return Precision; }
};

} // namespace teuthid

#endif // TEUTHID_FLOATMP_HPP