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

#define BOOST_TEST_MODULE teuthid
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <teuthid/floatmp.hpp>
#include <teuthid/system.hpp>

using namespace teuthid;

BOOST_AUTO_TEST_CASE(class_teuthid_floatmp) {
  float256_t __x1;
  floatmp<500> __x2;
  floatmp<100> __x3;
  float __x4;

  BOOST_TEST(__x1.precision() == float256_prec, "precision()");
  BOOST_TEST(__x2.precision() == 500, "precision()");
  BOOST_TEST(__x3.precision() == 100, "precision()");
  floatmp_round_t __rmode = __x1.rounding_mode();
  BOOST_TEST(static_cast<int>(__x1.rounding_mode(__rmode)) ==
                 static_cast<int>(__rmode),
             "floatmp::rounding_mode()");
  BOOST_TEST(__x1.is_finite(), "is_finite()");
  BOOST_TEST(system::is_finite(__x2), "system::is_finite()");
  BOOST_TEST(!__x2.is_infinite(), "is_infinite()");
  BOOST_TEST(!system::is_infinite(__x3), "system::is_infinite()");
  BOOST_TEST(!__x1.is_nan(), "is_nan()");
  BOOST_TEST(!system::is_nan(__x2), "system::is_nan()");
  BOOST_TEST(__x1.is_zero(), "is_zero()");
  BOOST_TEST(__x2.is_zero(), "is_zero()");
  BOOST_TEST(__x3.is_zero(), "is_zero()");
  BOOST_TEST(system::is_zero(__x2), "system::is_zero()");

  __x1 = -0.1, __x2 = 0.0, __x3 = 0.1;
  BOOST_TEST(!__x1.is_positive(), "is_positive()");
  BOOST_TEST(!__x2.is_positive(), "is_positive()");
  BOOST_TEST(__x3.is_positive(), "is_positive()");
  BOOST_TEST(__x1.is_negative(), "is_negative()");
  BOOST_TEST(!__x2.is_negative(), "is_negative()");
  BOOST_TEST(!__x3.is_negative(), "is_negative()");
  BOOST_TEST(!system::is_positive(__x1), "system::is_positive()");
  BOOST_TEST(!system::is_positive(__x2), "system::is_positive()");
  BOOST_TEST(system::is_positive(__x3), "system::is_positive()");
  BOOST_TEST(system::is_negative(__x1), "system::is_negative()");
  BOOST_TEST(!system::is_negative(__x2), "system::is_negative()");
  BOOST_TEST(!system::is_negative(__x3), "system::is_negative()");

  __x1 = 0, __x2 = 0, __x3 = 0;
  BOOST_TEST((__x1 == __x2), "operator==");
  BOOST_TEST((__x1 == (double)__x2), "operator==");
  BOOST_TEST(((double)__x1 == __x2), "operator==");
  BOOST_TEST(!(__x1 < __x2), "operator<");
  BOOST_TEST(!(__x1 < (double)__x2), "operator<");
  BOOST_TEST(!((double)__x1 < __x2), "operator<");
  BOOST_TEST(!(__x1 > __x2), "operator>");
  BOOST_TEST(!(__x1 > (double)__x2), "operator>");
  BOOST_TEST(!((double)__x1 > __x2), "operator>");
  BOOST_TEST((__x1 <= __x2), "operator<=");
  BOOST_TEST((__x1 <= (double)__x2), "operator<=");
  BOOST_TEST(((double)__x1 <= __x2), "operator<=");
  BOOST_TEST((__x1 >= __x2), "operator>=");
  BOOST_TEST((__x1 >= (double)__x2), "operator>=");
  BOOST_TEST(((double)__x1 >= __x2), "operator>=");
  BOOST_TEST((__x1 == __x3), "operator==");
  __x1 = 0.9999999999, __x3 = -0.9999999999;
  BOOST_TEST(!(__x1 == __x2), "operator==");
  BOOST_TEST(!(__x1 < __x2), "operator<");
  BOOST_TEST((__x1 > __x2), "operator>");
  BOOST_TEST(!(__x1 <= __x2), "operator<=");
  BOOST_TEST((__x1 >= __x2), "operator>=");
  BOOST_TEST(!(__x1 == __x3), "operator==");

  __x3 = __x1;
  BOOST_TEST((__x1 == __x3), "operator==");
  BOOST_TEST(system::equal_to(float(__x1), float(__x3)), "operator float()");
  BOOST_TEST(system::equal_to(double(__x1), double(__x3)), "operator double()");
  BOOST_TEST(system::equal_to((long double)(__x1), (long double)(__x3)),
             "operator long double()");
  BOOST_TEST(system::equal_to(float16_t(__x1), float16_t(__x3)),
             "operator float16_t()");
  BOOST_TEST(system::equal_to(float32_t(__x1), float32_t(__x3)),
             "operator float32_t()");
  BOOST_TEST(system::equal_to(float64_t(__x1), float64_t(__x3)),
             "operator float64_t()");
  BOOST_TEST(system::equal_to(float80_t(__x1), float80_t(__x3)),
             "operator float80_t()");
  BOOST_TEST(system::equal_to(float128_t(__x1), float128_t(__x3)),
             "operator float128_t()");
  BOOST_TEST(system::equal_to(float256_t(__x1), float256_t(__x3)),
             "operator float256_t()");

  BOOST_TEST((float256_t::zero() == 0), "floatmp::zero()");
  BOOST_TEST((float256_t::minus_one() == -1), "floatmp::minus_one()");
  BOOST_TEST((float256_t::plus_one() == 1), "floatmp::plus_one()");

  __x1 = 1.0, __x2 = 2.0, __x3 = 3.0;
  BOOST_TEST(!__x1.is_zero(), "is_zero()");
  BOOST_TEST(__x1.less_than(2), "less_than()");
  BOOST_TEST(__x1.less_than(3.0), "less_than()");
  BOOST_TEST((__x1 < 3), "operator<");
  __x1 += __x2;
  BOOST_TEST((__x1 == __x3), "add()");
  BOOST_TEST(__x1.equal_to(3), "equal_to()");
  BOOST_TEST((3 == __x1), "operator==()");
  BOOST_TEST((3.1 != __x1), "operator!=()");
  BOOST_TEST((3.1 > __x1), "operator>()");
  BOOST_TEST(__x1.equal_to(3.0), "equal_to()");
  BOOST_TEST((__x1 == 3.0), "operator==()");
  BOOST_TEST((__x1 != 2.9999999999), "operator!=()");
  BOOST_TEST(__x1.equal_to(__x3), "equal_to()");
  BOOST_TEST(__x2.less_than(__x1), "less_than()");
  __x1 += 0.1;
  BOOST_TEST(__x3.less_than(__x1), "less_than()");
  __x1 = 1.111, __x1 = -__x1;
  BOOST_TEST((__x1 == -1.111), "operator-");
  __x2 = -__x1;
  BOOST_TEST((__x2 == 1.111), "operator-");
  __x2 -= 1;
  BOOST_TEST((__x2 == 0.111), "operator-=");
  BOOST_TEST(__x1.precision() == float256_prec, "precision()");
  BOOST_TEST(__x2.precision() == 500, "precision()");
  __x2 *= 2.0;
  BOOST_TEST((__x2 == 0.222), "operator*=");
  __x2 /= 2;
  BOOST_TEST((__x2 == 0.111), "operator/=");
  BOOST_TEST(__x2.precision() == 500, "precision()");

  __x1 = 1.111, __x2 = 2.222;
  __x3 = __x1 + __x2;
  BOOST_TEST((__x3 == 3.333), "operator+");
  BOOST_TEST(__x3.precision() == 100, "operator+");
  BOOST_TEST((__x2 == 2.222), "operator+");
  BOOST_TEST(__x2.precision() == 500, "operator+");
  __x3 = __x1 + 2.222;
  BOOST_TEST((__x3 == 3.333), "operator+");
  __x3 = 1.111 + __x2;
  BOOST_TEST((__x3 == 3.333), "operator+");
  BOOST_TEST(!__x3.is_zero(), "is_zero()");

  __x1 = 1.111, __x2 = 3.333;
  __x3 = __x2 - __x1;
  BOOST_TEST((__x3 == 2.222), "operator-");
  BOOST_TEST(__x3.precision() == 100, "operator-");
  BOOST_TEST((__x2 == 3.333), "operator-");
  BOOST_TEST(__x2.precision() == 500, "operator-");
  __x3 = __x2 - 2.222;
  BOOST_TEST((__x3 == 1.111), "operator-");
  __x3 = 4.444 - __x2;
  BOOST_TEST((__x3 == 1.111), "operator-");
  BOOST_TEST(!__x3.is_zero(), "is_zero()");

  __x1 = 1.111, __x2 = 2;
  __x3 = __x1 * __x2;
  BOOST_TEST((__x3 == 2.222), "operator*");
  BOOST_TEST(__x3.precision() == 100, "operator*");
  __x3 = __x1 * 2;
  BOOST_TEST((__x3 == 2.222), "operator*");
  __x3 = 2 * __x1;
  BOOST_TEST((__x3 == 2.222), "operator*");
  BOOST_TEST(!__x3.is_zero(), "is_zero()");

  __x1 = 3.333, __x2 = 3;
  __x3 = __x1 / __x2;
  BOOST_TEST((__x3 == 1.111), "operator/");
  BOOST_TEST(__x3.precision() == 100, "operator/");
  __x3 = __x1 / 3.0;
  BOOST_TEST((__x3 == 1.111), "operator/");
  __x3 = 6.666 / __x2;
  BOOST_TEST((__x3 == 2.222), "operator/");
  BOOST_TEST(!__x3.is_zero(), "is_zero()");

  __x1 = 1.111, __x2 = 2.222;
  __x1.swap(__x2);
  BOOST_TEST((__x1 == 2.222), "swap()");
  BOOST_TEST((__x2 == 1.111), "swap()");
  BOOST_TEST(__x1.precision() == float256_prec, "precision()");
  BOOST_TEST(__x2.precision() == 500, "precision()");
  std::swap(__x1, __x2);
  BOOST_TEST((__x2 == 2.222), "std::swap()");
  BOOST_TEST((__x1 == 1.111), "std::swap()");

  __x1 = 1.111;
  BOOST_TEST((__x1.abs(__x1) == 1.111), "abs()");
  __x1 = -1.111;
  BOOST_TEST((__x2.abs(__x1) == 1.111), "abs()");
  BOOST_TEST((std::abs(__x1) == 1.111), "std::abs()");
  __x1 = -1.111;
  BOOST_TEST((std::abs(__x1) == 1.111), "std::abs()");
  BOOST_TEST((std::fabs(__x1) == 1.111), "std::fabs()");

  __x1 = 33.33, __x2 = 9.9;
  BOOST_TEST((__x1.fmod(__x1, __x2) == std::fmod(double(__x1), double(__x2))),
             "fmod()");
  __x1 = -33.33;
  BOOST_TEST((__x2.fmod(__x1, __x2) == std::fmod(double(__x1), double(__x2))),
             "fmod()");
  __x1 = -999.99, __x2 = 9.9;
  BOOST_TEST((std::fmod(__x1, __x2) == std::fmod(double(__x1), double(__x2))),
             "std::fmod()");

  __x1 = 33.33, __x2 = 8.8;
  BOOST_TEST((__x1.remainder(__x1, __x2) ==
              std::remainder(double(__x1), double(__x2))),
             "remainder()");
  __x1 = -22.22;
  BOOST_TEST((__x2.remainder(__x1, __x2) ==
              std::remainder(double(__x1), double(__x2))),
             "remainder()");
  __x1 = 1.234, __x2 = 8.8;
  BOOST_TEST((std::remainder(__x1, __x2) ==
              std::remainder(double(__x1), double(__x2))),
             "std::remainder()");

  __x1 = 1.1111, __x2 = -2.2222, __x3 = 3.3333;
  BOOST_TEST((__x1.fma(__x2, __x2, __x3) ==
              std::fma(double(__x2), double(__x2), double(__x3))),
             "fma()");
  BOOST_TEST((std::fma(__x1, __x2, __x3) ==
              std::fma(double(__x1), double(__x2), double(__x3))),
             "std::fma()");
  BOOST_TEST((std::fma(__x2, __x3, __x1) ==
              std::fma(double(__x2), double(__x3), double(__x1))),
             "std::fma()");

  __x1 = 1.1111, __x2 = -2.2222;
  BOOST_TEST((__x1.fmax(__x1, __x2) == __x1), "fmax()");
  BOOST_TEST((std::fmax(__x1, __x2) == __x1), "std::fmax()");
  BOOST_TEST((std::fmax(__x2, __x1) == __x1), "std::fmax()");
  BOOST_TEST((std::fmax(__x2, __x2) == __x2), "std::fmax()");

  __x1 = 1.1111, __x2 = -2.2222;
  BOOST_TEST((__x2.fmin(__x1, __x2) == __x2), "fmin()");
  BOOST_TEST((std::fmin(__x1, __x2) == __x2), "std::fmin()");
  BOOST_TEST((std::fmin(__x2, __x1) == __x2), "std::fmin()");
  BOOST_TEST((std::fmin(__x1, __x1) == __x1), "std::fmin()");

  __x1 = 9.9999, __x2 = 5.5555;
  BOOST_TEST((__x3.fdim(__x1, __x2) == std::fdim(double(__x1), double(__x2))),
             "fdim()");
  BOOST_TEST((__x3.fdim(__x2, __x1) == std::fdim(double(__x2), double(__x1))),
             "fdim()");
  BOOST_TEST((std::fdim(__x1, __x2) == std::fdim(double(__x1), double(__x2))),
             "std::fdim()");
  BOOST_TEST((std::fdim(__x2, __x1) == std::fdim(double(__x2), double(__x1))),
             "std::fdim()");

  __x2 = -9.9999;
  BOOST_TEST((__x1.exp(__x2) == std::exp(double(__x2))), "exp()");
  BOOST_TEST((std::exp(__x2) == std::exp(double(__x2))), "std::exp()");
  BOOST_TEST((__x1.exp2(__x2) == std::exp2(double(__x2))), "exp2()");
  BOOST_TEST((std::exp2(__x2) == std::exp2(double(__x2))), "std::exp2()");
  BOOST_TEST((__x1.expm1(__x2) == std::expm1(double(__x2))), "expm1()");
  BOOST_TEST((std::expm1(__x2) == std::expm1(double(__x2))), "std::expm1()");

  __x2 = 99.9999;
  BOOST_TEST((__x1.log(__x2) == std::log(double(__x2))), "log()");
  BOOST_TEST((std::log(__x2) == std::log(double(__x2))), "std::log()");
  BOOST_TEST((__x1.log10(__x2) == std::log10(double(__x2))), "log10()");
  BOOST_TEST((std::log10(__x2) == std::log10(double(__x2))), "std::log10()");
  BOOST_TEST((__x1.log2(__x2) == std::log2(double(__x2))), "log2()");
  BOOST_TEST((std::log2(__x2) == std::log2(double(__x2))), "std::log2()");
  BOOST_TEST((__x1.log1p(__x2) == std::log1p(double(__x2))), "log1p()");
  BOOST_TEST((std::log1p(__x2) == std::log1p(double(__x2))), "std::log1p()");

  __x2 = 99.9999;
  BOOST_TEST((__x1.ceil(__x2) == std::ceil(double(__x2))), "ceil()");
  BOOST_TEST((std::ceil(__x2) == std::ceil(double(__x2))), "std::ceil()");
  BOOST_TEST((__x1.floor(__x2) == std::floor(double(__x2))), "floor()");
  BOOST_TEST((std::floor(__x2) == std::floor(double(__x2))), "std::floor()");
  __x2 = 99.0001;
  BOOST_TEST((__x1.ceil(__x2) == std::ceil(double(__x2))), "ceil()");
  BOOST_TEST((std::ceil(__x2) == std::ceil(double(__x2))), "std::ceil()");
  BOOST_TEST((__x1.floor(__x2) == std::floor(double(__x2))), "floor()");
  BOOST_TEST((std::floor(__x2) == std::floor(double(__x2))), "std::floor()");

#ifdef TEUTHID_HAVE_INT_128
  __x1 = static_cast<int128_t>(INT64_MAX) * 10;
  __x2 = static_cast<int128_t>(INT64_MAX) * 10;
  BOOST_TEST(!system::to_string(__x1).empty());
  BOOST_TEST(!system::to_string(__x2).empty());
  BOOST_TEST((__x1 == __x2), "operator==");
  __x2 = 0;
  BOOST_TEST(__x2.less_than(static_cast<int128_t>(INT64_MAX) * 10),
             "less_than()");
  __x2 += (static_cast<int128_t>(INT64_MAX) * 10);
  BOOST_TEST((__x1 == __x2), "add()");
  __x2 = static_cast<int128_t>(INT64_MAX) * 11;
  BOOST_TEST(__x1.less_than(static_cast<int128_t>(INT64_MAX) * 11),
             "less_than()");
  BOOST_TEST((__x2 < static_cast<int128_t>(INT64_MAX) * 13), "operator<");
  BOOST_TEST((__x1 < __x2), "operator<");
  __x3 = __x2;
  __x3 += 0.9999999999;
  BOOST_TEST(__x2.less_than(__x3), "less_than()");

  __x1 = static_cast<int128_t>(INT64_MIN) * 10;
  __x2 = static_cast<int128_t>(INT64_MIN) * 10;
  BOOST_TEST((__x1 == __x2), "operator==");
  BOOST_TEST((__x1 == (static_cast<int128_t>(INT64_MIN)) * 10), "operator==");
  BOOST_TEST((__x1 != (static_cast<int128_t>(INT64_MIN)) * 11), "operator!=");
  BOOST_TEST((((static_cast<int128_t>(INT64_MIN)) * 10) == __x2), "operator==");
  BOOST_TEST((((static_cast<int128_t>(INT64_MIN)) * 9) != __x2), "operator!=");
  __x3 = 0;
  __x3 += (static_cast<int128_t>(INT64_MIN) * 10);
  BOOST_TEST((__x1 == __x3), "add()");
  __x2 = static_cast<int128_t>(INT64_MIN) * 11;
  BOOST_TEST((__x1 > __x2), "operator>");
  BOOST_TEST(__x2.less_than(INT64_MIN), "less_than()");

  __x1 = static_cast<uint128_t>(UINT64_MAX) * 10;
  __x2 = static_cast<uint128_t>(UINT64_MAX) * 10;
  BOOST_TEST(!system::to_string(__x1).empty());
  BOOST_TEST(!system::to_string(__x2).empty());
  BOOST_TEST((__x1 == __x2), "operator==");
  __x2 = static_cast<uint128_t>(UINT64_MAX) * 11;
  BOOST_TEST((__x1 < __x2), "operator<");
  __x3 = 0.9999999999;
  BOOST_TEST(__x3.less_than(__x2), "less_than()");
  __x3 += __x2;
  BOOST_TEST(__x2.less_than(__x3), "less_than()");
#endif // TEUTHID_HAVE_INT_128
}