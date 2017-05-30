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
#include <teuthid/system.hpp>

using namespace teuthid;

std::atomic_int floatmp_base::round_mode_(mpfr_get_default_rounding_mode());

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef TEUTHID_HAVE_INT_128
template <> void floatmp_base::assign(const int128_t &value) {
  mpfr_t __result;
  mpfr_init2(__result, mpfr_get_prec(value_));
  int64_t __mod = value % INT64_MAX;
  mpfr_set_sj(value_, INT64_MAX, static_cast<mpfr_rnd_t>(rounding_mode()));
  mpfr_mul_si(__result, value_, __mod,
              static_cast<mpfr_rnd_t>(rounding_mode()));
  mpfr_set(value_, __result, static_cast<mpfr_rnd_t>(rounding_mode()));
  mpfr_clear(__result);
}

template <> void floatmp_base::assign(const uint128_t &value) {
  mpfr_t __result;
  mpfr_init2(__result, mpfr_get_prec(value_));
  uint64_t __mod = value % UINT64_MAX;
  mpfr_set_uj(value_, UINT64_MAX, static_cast<mpfr_rnd_t>(rounding_mode()));
  mpfr_mul_ui(__result, value_, __mod,
              static_cast<mpfr_rnd_t>(rounding_mode()));
  mpfr_set(value_, __result, static_cast<mpfr_rnd_t>(rounding_mode()));
  mpfr_clear(__result);
}
#endif // TEUTHID_HAVE_INT_128
#endif // DOXYGEN_SHOULD_SKIP_THIS

bool floatmp_base::equal_to(const floatmp_base &value) const {
  return system::equal_to(value_, value.value_);
}

bool floatmp_base::less_than(const floatmp_base &value) const {
  return system::less_than(value_, value.value_);
}
