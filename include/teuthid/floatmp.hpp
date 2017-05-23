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
  floatmp_base() = default;
  // floatmp_base(const floatmp_base &) = default;
  // floatmp_base(floatmp_base &&) = default;
  virtual ~floatmp_base() {}
  // floatmp_base &operator=(const floatmp_base &) = default;
  // floatmp_base &operator=(floatmp_base &&) = default;

  static floatmp_round_t rounding_mode();

private:
  static std::mutex round_mode_mutex_;
};

template <std::size_t Precision> class floatmp : public floatmp_base {
public:
  floatmp() {}
  virtual ~floatmp() {}
  constexpr std::size_t precision() const noexcept { return Precision; }

private:
  mpfr_t value_;
};

} // namespace teuthid

#endif // TEUTHID_FLOATMP_HPP