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
#include <teuthid/system.hpp>

using namespace teuthid;

BOOST_AUTO_TEST_CASE(class_teuthid_system) {

  std::string __s;

  BOOST_TEST(system::major_version() == TEUTHID_MAJOR_VERSION,
             "major_version()");
  BOOST_TEST(system::minor_version() == TEUTHID_MINOR_VERSION,
             "minor_version()");
  BOOST_TEST(system::patch_version() == TEUTHID_PATCH_VERSION,
             "patch_version()");
  BOOST_TEST(system::soversion() == TEUTHID_SOVERSION, "soversion()");
  BOOST_TEST(system::version() == std::string(TEUTHID_VERSION), "version()");

  int major_ver = system::major_version();
  int minor_ver = system::minor_version();
  BOOST_TEST(system::check_version(major_ver, minor_ver), "check_version()");
  BOOST_TEST(!system::check_version(major_ver + 1, minor_ver),
             "check_version()");
  BOOST_TEST(!system::check_version(major_ver, minor_ver + 1),
             "check_version()");
  BOOST_TEST(system::check_version(major_ver, minor_ver - 1),
             "check_version()");

#if defined(TEUTHID_WITH_OPENCL)
  BOOST_TEST(system::has_cl_backend(), "has_cl_backend()");
  BOOST_TEST(system::uses_cl_backend(), "uses_cl_backend()");
  BOOST_TEST(!system::uses_cl_backend(false), "uses_cl_backend(bool)");
  BOOST_TEST(!system::uses_cl_backend(), "uses_cl_backend()");
  BOOST_TEST(system::uses_cl_backend(true), "uses_cl_backend(bool)");
#else
  BOOST_TEST(!system::has_cl_backend(), "has_cl_backend()");
  BOOST_TEST(!system::uses_cl_backend(), "uses_cl_backend()");
  BOOST_TEST(!system::uses_cl_backend(true), "uses_cl_backend(bool)");
  BOOST_TEST(!system::uses_cl_backend(false), "uses_cl_backend(bool)");
#endif

  int __x = 13;
  BOOST_TEST((system::to_string(__x) == "13"), "to_string(int)");
  size_t __size = 13;
  BOOST_TEST((system::to_string(__size) == "13"), "to_string(size_t)");
  intptr_t __intptr = -13;
  BOOST_TEST((system::to_string(__intptr) == "-13"), "to_string(intptr_t)");
  int8_t __int8 = -128;
  BOOST_TEST((system::to_string(__int8) == "-128"), "to_string(int8_t)");
  uint8_t __uint8 = 255;
  BOOST_TEST((system::to_string(__uint8) == "255"), "to_string(uint8_t)");
  int64_t __int64 = -9223372036854775807;
  BOOST_TEST((system::to_string(__int64) == "-9223372036854775807"),
             "to_string(int64_t)");
  uint64_t __uint64 = (uint64_t)9223372036854775807;
  BOOST_TEST((system::to_string(__uint64) == "9223372036854775807"),
             "to_string(uint64_t)");

#ifdef TEUTHID_HAVE_INT_128
  int128_t __int_128 = static_cast<int128_t>(INT64_MIN) - 13;
  int128_t __int_128_x = 0;
  __s = system::to_string(__int_128);
  BOOST_TEST(!__s.empty(), "system::to_string(int128_t)");
  system::from_string(__s, __int_128_x);
  BOOST_TEST((__int_128 == __int_128_x), "system::from_string(int128_t)");
  __int_128 = static_cast<int128_t>(INT64_MAX) + 13;
  __s = system::to_string(__int_128);
  BOOST_TEST(!__s.empty(), "system::to_string(int128_t)");
  system::from_string(__s, __int_128_x);
  BOOST_TEST((__int_128 == __int_128_x), "system::from_string(int128_t)");
  __int_128 = static_cast<int128_t>(INT64_MIN) * 13;
  __s = system::to_string(__int_128);
  BOOST_TEST(!__s.empty(), "system::to_string(int128_t)");
  system::from_string(__s, __int_128_x);
  BOOST_TEST((__int_128 == __int_128_x), "system::from_string(int128_t)");
  __int_128 = static_cast<int128_t>(INT64_MAX) * 13;
  __s = system::to_string(__int_128);
  BOOST_TEST(!__s.empty(), "system::to_string(int128_t)");
  system::from_string(__s, __int_128_x);
  BOOST_TEST((__int_128 == __int_128_x), "system::from_string(int128_t)");

  uint128_t __uint_128 = static_cast<uint128_t>(UINT64_MAX) + 13;
  uint128_t __uint_128_x = 0;
  __s = system::to_string(__uint_128);
  BOOST_TEST(!__s.empty(), "system::to_string(uint128_t)");
  system::from_string(__s, __uint_128_x);
  BOOST_TEST((__uint_128 == __uint_128_x), "system::from_string(uint128_t)");
  __uint_128 = static_cast<uint128_t>(UINT64_MAX) * 13;
  __s = system::to_string(__uint_128);
  BOOST_TEST(!__s.empty(), "system::to_string(uint128_t)");
  system::from_string(__s, __uint_128_x);
  BOOST_TEST((__uint_128 == __uint_128_x), "system::from_string(uint128_t)");
#endif // TEUTHID_HAVE_INT_128

  bool __false = false;
  bool __true = true;
  bool &__ref_true = __true;
  const bool &__const_ref_true = __true;
  BOOST_TEST((system::to_string(__false) == "false"), "to_string(bool)");
  BOOST_TEST((system::to_string(__true) == "true"), "to_string(bool)");
  BOOST_TEST((system::to_string(__ref_true) == "true"), "to_string(bool)");
  BOOST_TEST((system::to_string(__const_ref_true) == "true"),
             "to_string(bool)");

  char __char = 'Q';
  BOOST_TEST((system::to_string(__char)) == "Q", "to_string(char)");
  char __chars[] = {'t', 'e', 's', 't', '\0'};
  // BOOST_TEST((system::to_string(__chars)) == "test", "to_string(char[])");
  const char *__str = "test";
  BOOST_TEST((system::to_string(__str)) == "test", "to_string(const char*)");
  std::vector<std::string> __vector = {"1", "2", "3", "4"};
  BOOST_TEST((system::to_string(__vector)) == "1 2 3 4",
             "to_string(const std::vector<std::string> &");
  void *__ptr_void = reinterpret_cast<void *>(13);
  BOOST_TEST((system::to_string(__ptr_void)) == "13", "to_string(void*)");

  mpfr_t __mpfr;
  mpfr_init_set_ld(__mpfr, 1.2345, mpfr_get_default_rounding_mode());
  BOOST_TEST(!system::to_string(__mpfr).empty());
  system::format_float_output();
  BOOST_TEST((system::to_string(__mpfr) == "1.2345000000"),
             "system::format_float_output(10, false)");
  system::format_float_output(10, true);
  BOOST_TEST((system::to_string(__mpfr) == "1.2345000000e+00"),
             "system::format_float_output(10, true)");

  float256_t __fmp1, __fmp2;
  system::format_float_output();
  BOOST_TEST(!system::to_string(__fmp1).empty(), "system::to_string(floatmp)");
  BOOST_TEST(system::to_string(__fmp1) == system::to_string(__fmp2),
             "system::to_string(floatmp)");
  BOOST_TEST(system::equal_to(__fmp1, __fmp2));

  bool __boolval, __boolval2 = false;
  BOOST_TEST(!system::from_string("F A L S E", __boolval),
             "system::from_string(bool)");
  BOOST_TEST(system::is_finite(__boolval), "system::is_finite(bool)");
  BOOST_TEST(!system::is_infinite(__boolval), "system::is_infinite(bool)");
  BOOST_TEST(!system::is_nan(__boolval), "system::is_nan(bool)");
  BOOST_TEST(system::is_zero(__boolval), "system::is_zero(bool)");
  BOOST_TEST(system::from_string("true", __boolval),
             "system::from_string(bool)");
  BOOST_TEST(system::from_string("1", __boolval), "system::from_string(bool)");
  BOOST_TEST(!system::from_string("0", __boolval), "system::from_string(bool)");
  __boolval = true;
  system::swap(__boolval, __boolval2);
  BOOST_TEST(!__boolval, "system::swap(bool)");
  BOOST_TEST(__boolval2, "system::swap(bool)");

  int8_t __int8_val, __int8_val2 = 13;
  BOOST_TEST(system::from_string("-127", __int8_val) == -127,
             "system::from_string(int8_t)");
  BOOST_TEST(system::is_finite(__int8_val), "system::is_finite(int8_t)");
  BOOST_TEST(!system::is_infinite(__int8_val), "system::is_infinite(int8_t)");
  BOOST_TEST(!system::is_nan(__int8_val), "system::is_nan(int8_t)");
  BOOST_TEST(!system::is_zero(__int8_val), "system::is_zero(int8_t)");
  system::swap(__int8_val, __int8_val2);
  BOOST_TEST((__int8_val == 13), "system::swap(int8_t)");
  BOOST_TEST((__int8_val2 == -127), "system::swap(int8_t)");

  int16_t __int16_val;
  BOOST_TEST(system::from_string("-32768", __int16_val) == -32768,
             "system::from_string(int16_t)");
  BOOST_TEST(system::is_finite(__int16_val), "system::is_finite(int16_t)");
  BOOST_TEST(!system::is_infinite(__int16_val), "system::is_infinite(int16_t)");
  BOOST_TEST(!system::is_nan(__int16_val), "system::is_nan(int16_t)");
  BOOST_TEST(!system::is_zero(__int16_val), "system::is_zero(int16_t)");

  int32_t __int32_val;
  BOOST_TEST(system::from_string("-2147483648", __int32_val) == -2147483648,
             "system::from_string(int32_t)");
  BOOST_TEST(system::is_finite(__int32_val), "system::is_finite(int32_t)");
  BOOST_TEST(!system::is_infinite(__int32_val), "system::is_infinite(int32_t)");
  BOOST_TEST(!system::is_nan(__int32_val), "system::is_nan(int32_t)");
  BOOST_TEST(!system::is_zero(__int32_val), "system::is_zero(int32_t)");

  int64_t __int64_val;
  BOOST_TEST(system::from_string("-9223372036854775807", __int64_val) ==
                 -9223372036854775807,
             "system::from_string(int64_t)");
  BOOST_TEST(system::is_finite(__int64_val), "system::is_finite(int64_t)");
  BOOST_TEST(!system::is_infinite(__int64_val), "system::is_infinite(int64_t)");
  BOOST_TEST(!system::is_nan(__int64_val), "system::is_nan(int64_t)");
  BOOST_TEST(!system::is_zero(__int64_val), "system::is_zero(int64_t)");

  uint8_t __uint8_val;
  BOOST_TEST(system::from_string("255", __uint8_val) == 255,
             "system::from_string(uint8_t)");
  BOOST_TEST(system::is_finite(__uint8_val), "system::is_finite(uint8_t)");
  BOOST_TEST(!system::is_infinite(__uint8_val), "system::is_infinite(uint8_t)");
  BOOST_TEST(!system::is_nan(__uint8_val), "system::is_nan(uint8_t)");
  BOOST_TEST(!system::is_zero(__uint8_val), "system::is_zero(uint8_t)");

  uint16_t __uint16_val;
  BOOST_TEST(system::from_string("65535", __uint16_val) == 65535,
             "system::from_string(uint16_t)");
  BOOST_TEST(system::is_finite(__uint16_val), "system::is_finite(uint16_t)");
  BOOST_TEST(!system::is_infinite(__uint16_val),
             "system::is_infinite(uint16_t)");
  BOOST_TEST(!system::is_nan(__uint16_val), "system::is_nan(uint16_t)");
  BOOST_TEST(!system::is_zero(__uint16_val), "system::is_zero(uint16_t)");

  uint32_t __uint32_val;
  BOOST_TEST(system::from_string("4294967295", __uint32_val) == 4294967295,
             "system::from_string(uint32_t)");
  BOOST_TEST(system::is_finite(__uint32_val), "system::is_finite(uint32_t)");
  BOOST_TEST(!system::is_infinite(__uint32_val),
             "system::is_infinite(uint32_t)");
  BOOST_TEST(!system::is_nan(__uint32_val), "system::is_nan(uint32_t)");
  BOOST_TEST(!system::is_zero(__uint32_val), "system::is_zero(uint32_t)");

  uint64_t __uint64_val;
  BOOST_TEST(system::from_string("9223372036854775807", __uint64_val) ==
                 9223372036854775807,
             "system::from_string(uint64_t)");
  BOOST_TEST(system::is_finite(__uint64_val), "system::is_finite(uint64_t)");
  BOOST_TEST(!system::is_infinite(__uint64_val),
             "system::is_infinite(uint64_t)");
  BOOST_TEST(!system::is_nan(__uint64_val), "system::is_nan(uint64_t)");
  BOOST_TEST(!system::is_zero(__uint64_val), "system::is_zero(uint64_t)");

#ifdef TEUTHID_HAVE_INT_128
  int128_t __int128_val = (int128_t)INT64_MAX * 10;
  uint128_t __uint128_val = (uint128_t)UINT64_MAX * 10;
  BOOST_TEST((__int128_val == __int128_val));
  BOOST_TEST((__int128_val != __uint128_val));
  BOOST_TEST(system::is_finite(__int128_val), "system::is_finite(int128_t)");
  BOOST_TEST(system::is_finite(__uint128_val), "system::is_finite(int128_t)");
  BOOST_TEST(!system::is_infinite(__int128_val),
             "system::is_infinite(int128_t)");
  BOOST_TEST(!system::is_infinite(__uint128_val),
             "system::is_infinite(int128_t)");
  BOOST_TEST(!system::is_nan(__int128_val), "system::is_nan(int128_t)");
  BOOST_TEST(!system::is_nan(__uint128_val), "system::is_nan(int128_t)");
  BOOST_TEST(!system::is_zero(__int128_val), "system::is_zero(int128_t)");
  BOOST_TEST(!system::is_zero(__uint128_val), "system::is_zero(int128_t)");
  int128_t __int128_val2 = -13;
  system::swap(__int128_val, __int128_val2);
  BOOST_TEST((__int128_val == -13), "system::swap(int128_t)");
  uint128_t __uint128_val2 = 13;
  system::swap(__uint128_val, __uint128_val2);
  BOOST_TEST((__uint128_val == 13), "system::swap(uint128_t)");
#endif // TEUTHID_HAVE_INT_128

  float __float_val, __float_val2 = 9.9;
  system::from_string("1.2345", __float_val);
  BOOST_TEST(system::is_finite(__float_val), "system::is_finite(float)");
  BOOST_TEST(!system::is_infinite(__float_val), "system::is_infinite(float)");
  BOOST_TEST(!system::is_nan(__float_val), "system::is_nan(float)");
  BOOST_TEST(!system::is_zero(__float_val), "system::is_zero(float)");
  BOOST_TEST(system::equal_to(__float_val, __float_val),
             "system::from_string(float)");
  BOOST_TEST(!system::less_than(__float_val, __float_val),
             "system::from_string(float)");
  BOOST_TEST(system::equal_to(__float_val, (float)1.2345),
             "system::equal_to(float)");
  BOOST_TEST(!system::less_than(__float_val, (float)1.2345),
             "system::less_than(float)");
  BOOST_TEST(!system::equal_to(__float_val, (float)1.2346),
             "system::equal_to(float)");
  BOOST_TEST(system::less_than(__float_val, (float)1.2346),
             "system::less_than(float)");
  BOOST_TEST(!system::less_than((float)1.2346, __float_val),
             "system::less_than(float)");
  system::swap(__float_val, __float_val2);
  BOOST_TEST(system::equal_to(__float_val, (float)9.9), "system::swap(float)");

  double __double_val;
  system::from_string("1.23456", __double_val);
  BOOST_TEST(system::is_finite(__double_val), "system::is_finite(double)");
  BOOST_TEST(!system::is_infinite(__double_val), "system::is_infinite(double)");
  BOOST_TEST(!system::is_nan(__double_val), "system::is_nan(double)");
  BOOST_TEST(!system::is_zero(__double_val), "system::is_zero(double)");
  BOOST_TEST(system::equal_to(__double_val, __double_val),
             "system::from_string(double)");
  BOOST_TEST(!system::less_than(__double_val, __double_val),
             "system::from_string(double)");
  BOOST_TEST(system::equal_to(__double_val, (double)1.23456),
             "system::equal_to(double)");
  BOOST_TEST(!system::less_than(__double_val, (double)1.23456),
             "system::less_than(double)");
  BOOST_TEST(!system::equal_to(__double_val, (double)1.23457),
             "system::equal_to(double)");
  BOOST_TEST(system::less_than(__double_val, (double)1.23457),
             "system::less_than(double)");
  BOOST_TEST(!system::less_than((double)1.23457, __double_val),
             "system::less_than(double)");

  long double __ldouble_val;
  system::from_string("1.234567", __ldouble_val);
  BOOST_TEST(system::is_finite(__ldouble_val),
             "system::is_finite(long double)");
  BOOST_TEST(!system::is_infinite(__ldouble_val),
             "system::is_infinite(long double)");
  BOOST_TEST(!system::is_nan(__ldouble_val), "system::is_nan(long double)");
  BOOST_TEST(!system::is_zero(__ldouble_val), "system::is_zero(long double)");
  BOOST_TEST(system::equal_to(__ldouble_val, __ldouble_val),
             "system::from_string(long double)");
  BOOST_TEST(!system::less_than(__ldouble_val, __ldouble_val),
             "system::from_string(long double)");
  BOOST_TEST(system::equal_to(__ldouble_val, (long double)1.234567),
             "system::equal_to(long double)");
  BOOST_TEST(!system::less_than(__ldouble_val, (long double)1.234567),
             "system::less_than(long double)");
  BOOST_TEST(!system::equal_to(__ldouble_val, (long double)1.234568),
             "system::equal_to(long double)");
  BOOST_TEST(system::less_than(__ldouble_val, (long double)1.234568),
             "system::less_than(long double)");
  BOOST_TEST(!system::less_than((long double)1.234568, __ldouble_val),
             "system::less_than(long double)");

  system::from_string("1.2345", __mpfr);
  BOOST_TEST(system::is_finite(__mpfr), "system::is_finite(mpfr_t)");
  BOOST_TEST(!system::is_infinite(__mpfr), "system::is_infinite(mpfr_t)");
  BOOST_TEST(!system::is_nan(__mpfr), "system::is_nan(mpfr_t)");
  BOOST_TEST(!system::is_zero(__mpfr), "system::is_zero(mpfr_t)");
  BOOST_TEST(mpfr_cmp_ld(__mpfr, (long double)1.2345) == 0,
             "system::from_string(mpfr_t)");
  BOOST_TEST(system::equal_to(__mpfr, __mpfr), "system::equal_to(mpfr_t)");
  BOOST_TEST(!system::less_than(__mpfr, __mpfr), "system::less_than(mpfr_t)");
  mpfr_t __mpfr2;
  mpfr_init2(__mpfr2, mpfr_get_default_prec());
  system::from_string("9.9999", __mpfr2);
  system::swap(__mpfr, __mpfr2);
  BOOST_TEST(mpfr_cmp_ld(__mpfr, (long double)9.9999) == 0,
             "system::swap(mpfr_t)");

  floatmp<200> __floatmp1;
  floatmp<300> __floatmp2;
  BOOST_TEST((system::from_string("1.2345", __floatmp1) ==
              (system::from_string("1.2345", __floatmp2))),
             "system::from_string(floatmp)");
  BOOST_TEST((system::from_string("1.2345", __floatmp1) !=
              (system::from_string("1.2346", __floatmp2))),
             "system::from_string(floatmp)");
  BOOST_TEST(system::is_finite(__floatmp1), "system::is_finite(floatmp)");
  BOOST_TEST(!system::is_infinite(__floatmp1), "system::is_infinite(floatmp)");
  BOOST_TEST(!system::is_nan(__floatmp1), "system::is_nan(floatmp)");
  BOOST_TEST(!system::is_zero(__floatmp1), "system::is_zero(floatmp)");
  system::swap(__floatmp1, __floatmp2);
  BOOST_TEST((__floatmp1 == 1.2346), "system::swap(floatmp)");

  std::string __text = "";
  std::vector<std::string> __strvector;
  system::split_string(__text, __strvector);
  BOOST_TEST(system::split_string(__text, __strvector) == 0,
             "system::split_string()");
  __text = " ";
  BOOST_TEST(system::split_string(__text, __strvector) == 0,
             "system::split_string()");
  __text = "  ";
  BOOST_TEST(system::split_string(__text, __strvector) == 0,
             "system::split_string()");
  __text = "x";
  BOOST_TEST(system::split_string(__text, __strvector) == 1,
             "system::split_string()");
  __text = "x ";
  BOOST_TEST(system::split_string(__text, __strvector) == 1,
             "system::split_string()");
  __text = " x";
  BOOST_TEST(system::split_string(__text, __strvector) == 1,
             "system::split_string()");
  __text = "x x";
  BOOST_TEST(system::split_string(__text, __strvector) == 2,
             "system::split_string()");
  __text = "x  x";
  BOOST_TEST(system::split_string(__text, __strvector) == 2,
             "system::split_string()");
  __text = "x xy 123   4";
  BOOST_TEST(system::split_string(__text, __strvector) == 4,
             "system::split_string()");

  mpfr_clear(__mpfr);
  mpfr_clear(__mpfr2);
}
