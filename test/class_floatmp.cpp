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
  float256_t __x2 = __x1;
  floatmp<100> __x3((int8_t)0);

  BOOST_TEST(__x1.precision() == float256_prec, "precision()");
  BOOST_TEST(__x3.precision() == 100, "precision()");
  floatmp_round_t __rmode = __x1.rounding_mode();
  BOOST_TEST(static_cast<int>(__x1.rounding_mode(__rmode)) ==
                 static_cast<int>(__rmode),
             "floatmp::rounding_mode()");

  system::format_float_output();
  BOOST_TEST((__x1 == __x2), "operator==");
  BOOST_TEST(!(__x1 < __x2), "operator<");
  BOOST_TEST(!(__x1 > __x2), "operator>");
  BOOST_TEST((__x1 <= __x2), "operator<=");
  BOOST_TEST((__x1 >= __x2), "operator>=");
  BOOST_TEST((__x1 == __x3), "operator==");
  __x1 = -13;
  BOOST_TEST((__x1 != __x3), "operator!=");
  BOOST_TEST((__x1 < __x3), "operator<");
  BOOST_TEST((__x1 <= __x3), "operator<=");
  BOOST_TEST(!(__x1 > __x3), "operator>");
  BOOST_TEST(!(__x1 >= __x3), "operator>=");
  __x1 = 12345;
  __x2 = __x1;
  BOOST_TEST((__x1 == __x2), "operator==");
  __x3 = -12345;
  BOOST_TEST((__x1 != __x3), "operator!=");
  BOOST_TEST((__x1 > __x3), "operator>");
  BOOST_TEST((__x3 < __x1), "operator<");
  __x1 = (float)1.234;
  __x3 = __x1;
  BOOST_TEST((__x1 == __x3), "operator==");
  __x1 = (double)1.2345;
  __x3 = __x1;
  BOOST_TEST((__x1 == __x3), "operator==");
  __x1 = (long double)1.23456;
  __x3 = __x1;
  BOOST_TEST((__x1 == __x3), "operator==");

#ifdef TEUTHID_HAVE_INT_128
  __x1 = static_cast<int128_t>(INT64_MAX) * 10;
  __x3 = __x1;
  BOOST_TEST(!system::to_string(__x1).empty());
  BOOST_TEST(!system::to_string(__x3).empty());
  BOOST_TEST((__x1 == __x3), "operator==");
  BOOST_TEST(!(__x1 < __x3), "operator<");
  __x1 = static_cast<uint128_t>(UINT64_MAX) * 10;
  __x3 = __x1;
  BOOST_TEST(!system::to_string(__x1).empty());
  BOOST_TEST(!system::to_string(__x3).empty());
  BOOST_TEST((__x1 == __x3), "operator==");
  BOOST_TEST(!(__x1 < __x3), "operator<");
#endif // TEUTHID_HAVE_INT_128
  //
}