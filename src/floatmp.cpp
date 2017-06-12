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

bool floatmp_base::equal_to(const floatmp_base &value) const {
  return system::equal_to(value_, value.value_);
}

bool floatmp_base::less_than(const floatmp_base &value) const {
  return system::less_than(value_, value.value_);
}

void floatmp_base::fmod(const floatmp_base &value) {
  if (value.is_zero())
    throw std::overflow_error("zero divisor of fmod()");
  else
    mpfr_fmod(value_, c_mpfr(), value.value_,
              static_cast<mpfr_rnd_t>(rounding_mode()));
}
