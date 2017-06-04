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

  system::format_float_output();

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
  __x1 = 0.9999999999;
  __x3 = -0.9999999999;
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

  __x1 = 1.0;
  __x2 = 2.0;
  __x3 = 3.0;
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
  __x1 = 1.111;
  __x1 = -__x1;
  BOOST_TEST((__x1 == -1.111), "operator-");
  __x2 = -__x1;
  BOOST_TEST((__x2 == 1.111), "operator-");
  BOOST_TEST(__x1.precision() == float256_prec, "precision()");
  BOOST_TEST(__x2.precision() == 500, "precision()");

  __x1 = 1.111;
  __x2 = 2.222;
  __x3 = __x1 + __x2;
  BOOST_TEST((__x3 == 3.333), "operator+");
  BOOST_TEST(__x3.precision() == 100, "operator+");
  BOOST_TEST((__x2 == 2.222), "operator+");
  BOOST_TEST(__x2.precision() == 500, "operator+");
  __x3 = __x1 + 2.222;
  BOOST_TEST((__x3 == 3.333), "operator+");
  __x3 = 1.111 + __x2;
  BOOST_TEST((__x3 == 3.333), "operator+");

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