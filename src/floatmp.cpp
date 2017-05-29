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

floatmp_base::floatmp_base(std::size_t precision) {
  mpfr_init2(value_, precision);
  mpfr_set_zero(value_, 1);
}

floatmp_base::floatmp_base(std::size_t precision, const floatmp_base &value) {
  mpfr_init2(value_, precision);
  mpfr_set(value_, value.c_mpfr(), static_cast<mpfr_rnd_t>(rounding_mode()));
}

floatmp_base::~floatmp_base() { mpfr_clear(value_); }

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define __TEUTHID_FLOATMP_ASSIGN_NUMBER(TYPE, FUN)                             \
  template <> void floatmp_base::assign(const TYPE &value) {                   \
    FUN(value_, value, static_cast<mpfr_rnd_t>(rounding_mode()));              \
  }

__TEUTHID_FLOATMP_ASSIGN_NUMBER(int8_t, mpfr_set_sj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(int16_t, mpfr_set_sj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(int32_t, mpfr_set_sj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(int64_t, mpfr_set_sj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(uint8_t, mpfr_set_uj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(uint16_t, mpfr_set_uj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(uint32_t, mpfr_set_uj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(uint64_t, mpfr_set_uj);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(float, mpfr_set_flt);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(double, mpfr_set_d);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(long double, mpfr_set_ld);
__TEUTHID_FLOATMP_ASSIGN_NUMBER(mpfr_t, mpfr_set);
#undef __TEUTHID_FLOATMP_ASSIGN_NUMBER

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
  return system::equal_to(value_, value.c_mpfr());
}
