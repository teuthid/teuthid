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

#include <teuthid/floatmp.hpp>

using namespace teuthid;

mpfr_rnd_t floatmp_base::round_mode_ = mpfr_get_default_rounding_mode();
std::mutex floatmp_base::round_mode_mutex_;

floatmp_base::floatmp_base(std::size_t precision) {
  mpfr_init2(value_, precision);
  mpfr_set_zero(value_, 1);
}

floatmp_base::floatmp_base(std::size_t precision, const floatmp_base &value) {
  mpfr_init2(value_, precision);
  std::lock_guard<std::mutex> lock(floatmp_base::round_mode_mutex_);
  mpfr_set(value_, value.c_mpfr(), round_mode_);
}

floatmp_base::~floatmp_base() { mpfr_clear(value_); }

floatmp_round_t floatmp_base::rounding_mode() {
  std::lock_guard<std::mutex> lock(floatmp_base::round_mode_mutex_);
  return static_cast<floatmp_round_t>(floatmp_base::round_mode_);
}

floatmp_round_t floatmp_base::rounding_mode(floatmp_round_t mode) {
  std::lock_guard<std::mutex> lock(floatmp_base::round_mode_mutex_);
  mpfr_rnd_t __prev = floatmp_base::round_mode_;
  floatmp_base::round_mode_ = static_cast<mpfr_rnd_t>(mode);
  return static_cast<floatmp_round_t>(floatmp_base::round_mode_);
}
