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

#include <stdexcept>

#include <teuthid/floatmp.hpp>
#include <teuthid/system.hpp>

using namespace teuthid;

std::atomic_int floatmp_base::round_mode_(mpfr_get_default_rounding_mode());
const floatmp_base floatmp_base::zero_(floatmp_base::max_precision());
const floatmp_base floatmp_base::minus_one_(floatmp_base::max_precision(),
                                            static_cast<long double>(-1));
const floatmp_base floatmp_base::plus_one_(floatmp_base::max_precision(),
                                           static_cast<long double>(1));

bool floatmp_base::equal_to(const floatmp_base &x) const {
  return system::equal_to(c_mpfr(), x.c_mpfr());
}

bool floatmp_base::less_than(const floatmp_base &x) const {
  return system::less_than(c_mpfr(), x.c_mpfr());
}

bool floatmp_base::is_integer() const {
  if (is_finite()) {
    floatmp_base __x(mpfr_get_prec(c_mpfr()));
    __x.trunc(*this);
    return equal_to(__x);
  }
  return false;
}

void floatmp_base::fmod(const floatmp_base &x, const floatmp_base &y) {
  if (y.is_zero())
    throw std::domain_error("invalid divisor of fmod()");
  mpfr_fmod(value_, x.c_mpfr(), y.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::remainder(const floatmp_base &x, const floatmp_base &y) {
  if (y.is_zero())
    throw std::domain_error("invalid divisor of remainder()");
  mpfr_remainder(value_, x.c_mpfr(), y.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::fmax(const floatmp_base &x, const floatmp_base &y) {
  const floatmp_base &__max = (system::less_than(x.value_, y.value_)) ? y : x;
  mpfr_set(value_, __max.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::fmin(const floatmp_base &x, const floatmp_base &y) {
  const floatmp_base &__min = (system::less_than(x.value_, y.value_)) ? x : y;
  mpfr_set(value_, __min.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::log(const floatmp_base &x) {
  if (!x.is_positive())
    throw std::domain_error("invalid arg of log()");
  mpfr_log(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::log10(const floatmp_base &x) {
  if (!x.is_positive())
    throw std::domain_error("invalid arg of log10()");
  mpfr_log10(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::log2(const floatmp_base &x) {
  if (!x.is_positive())
    throw std::domain_error("invalid arg of log2()");
  mpfr_log2(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::log1p(const floatmp_base &x) {
  if (!minus_one_.less_than(x))
    throw std::domain_error("invalid arg of log2()");
  mpfr_log1p(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::pow(const floatmp_base &x, const floatmp_base &y) {
  if (x.is_negative() && !y.is_integer())
    throw std::domain_error("invalid arg of pow()");
  if (x.is_zero() && !y.is_positive())
    throw std::domain_error("invalid arg of pow()");
  mpfr_pow(value_, x.c_mpfr(), y.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::sqrt(const floatmp_base &x) {
  if (x.is_negative())
    throw std::domain_error("invalid arg of sqrt()");
  mpfr_sqrt(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::asin(const floatmp_base &x) {
  if (x.less_than(minus_one_) || plus_one_.less_than(x))
    throw std::domain_error("invalid arg of asin()");
  mpfr_asin(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::acos(const floatmp_base &x) {
  if (x.less_than(minus_one_) || plus_one_.less_than(x))
    throw std::domain_error("invalid arg of asin()");
  mpfr_acos(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::acosh(const floatmp_base &x) {
  if (x.less_than(plus_one_))
    throw std::domain_error("invalid arg of acosh()");
  mpfr_acosh(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::atanh(const floatmp_base &x) {
  if (!(minus_one_.less_than(x) && x.less_than(plus_one_)))
    throw std::domain_error("invalid arg of atanh()");
  mpfr_atanh(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::tgamma(const floatmp_base &x) {
  if (x.is_zero())
    throw std::domain_error("invalid arg of tgamma()");
  if (x.is_integer() && x.is_negative())
    throw std::domain_error("invalid arg of tgamma()");
  mpfr_gamma(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::lgamma(const floatmp_base &x) {
  if (x.is_zero())
    throw std::domain_error("invalid arg of tgamma()");
  if (x.is_integer() && x.is_negative())
    throw std::domain_error("invalid arg of tgamma()");
  mpfr_lngamma(value_, x.c_mpfr(), mpfr_rnd_());
}

void floatmp_base::nextafter(const floatmp_base &x, const floatmp_base &y) {
  if (!x.is_finite())
    throw std::domain_error("invalid arg of nextafter()");
  mpfr_set(value_, x.c_mpfr(), mpfr_rnd_());
  mpfr_nexttoward(value_, y.c_mpfr());
}

void floatmp_base::nextabove(const floatmp_base &x) {
  if (!x.is_finite())
    throw std::domain_error("invalid arg of nextabove()");
  mpfr_set(value_, x.c_mpfr(), mpfr_rnd_());
  mpfr_nextabove(value_);
}

void floatmp_base::nextbelow(const floatmp_base &x) {
  if (!x.is_finite())
    throw std::domain_error("invalid arg of nextbelow()");
  mpfr_set(value_, x.c_mpfr(), mpfr_rnd_());
  mpfr_nextbelow(value_);
}
