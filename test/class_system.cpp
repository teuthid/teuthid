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
#include <chrono>

#include <teuthid/system.hpp>
#include <thread>

using namespace teuthid;

class test_thread {
public:
  void test() { system::use_clb(false); }
};

class test {
  std::thread t1, t2;

public:
  void call_threads() {
    t1 = std::thread(&test_thread::test, test_thread());
    t2 = std::thread(&test_thread::test, test_thread());
    t1.join();
    t2.join();
  }
};

BOOST_AUTO_TEST_CASE(class_teuthid_system) {
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
  BOOST_TEST(system::is_required_version(major_ver, minor_ver),
             "is_required_version()");
  BOOST_TEST(!system::is_required_version(major_ver + 1, minor_ver),
             "is_required_version()");
  BOOST_TEST(!system::is_required_version(major_ver, minor_ver + 1),
             "is_required_version()");
  BOOST_TEST(system::is_required_version(major_ver, minor_ver - 1),
             "is_required_version()");

  test __test = test();
#if defined(TEUTHID_WITH_OPENCL)
  BOOST_TEST(system::have_clb(), "have_clb()");
  BOOST_TEST(system::use_clb(), "use_clb()");
  BOOST_TEST(!system::use_clb(false), "use_clb(bool)");
  BOOST_TEST(!system::use_clb(), "use_clb()");
  BOOST_TEST(system::use_clb(true), "use_clb(bool)");
  __test.call_threads();
  std::this_thread::sleep_for(std::chrono::milliseconds(123));
  BOOST_TEST(system::use_clb(), "use_clb()");
#else
  BOOST_TEST(!system::have_clb(), "have_clb()");
  BOOST_TEST(!system::use_clb(), "use_clb()");
  BOOST_TEST(!system::use_clb(true), "use_clb(bool)");
  BOOST_TEST(!system::use_clb(false), "use_clb(bool)");
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
  int128_t __int_128 = (int128_t)INT64_MIN;
  BOOST_TEST(!system::to_string(__int_128).empty());
  __int_128 = (int128_t)INT64_MAX;
  BOOST_TEST(!system::to_string(__int_128).empty());
  __int_128 = (int128_t)INT64_MIN * 10;
  BOOST_TEST(!system::to_string(__int_128).empty());
  __int_128 = (int128_t)INT64_MAX * 10;
  BOOST_TEST(!system::to_string(__int_128).empty());
  uint128_t __uint_128 = (uint128_t)UINT64_MAX;
  BOOST_TEST(!system::to_string(__uint_128).empty());
  __uint_128 = (uint128_t)UINT64_MAX * 10;
  BOOST_TEST(!system::to_string(__uint_128).empty());
#endif
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

  bool __boolval;
  BOOST_TEST(!system::from_string("F A L S E", __boolval),
             "system::from_string(, bool)");
  BOOST_TEST(system::from_string("true", __boolval),
             "system::from_string(, bool)");
  int8_t __int8_val;
  BOOST_TEST(system::from_string("-127", __int8_val) == -127,
             "system::from_string(, int8_t)");
  int16_t __int16_val;
  BOOST_TEST(system::from_string("-32768", __int16_val) == -32768,
             "system::from_string(, int16_t)");
  int32_t __int32_val;
  BOOST_TEST(system::from_string("-2147483648", __int32_val) == -2147483648,
             "system::from_string(, int32_t)");
  int64_t __int64_val;
  BOOST_TEST(system::from_string("-9223372036854775807", __int64_val) ==
                 -9223372036854775807,
             "system::from_string(, int64_t)");

  uint8_t __uint8_val;
  BOOST_TEST(system::from_string("255", __uint8_val) == 255,
             "system::from_string(, uint8_t)");
  uint16_t __uint16_val;
  BOOST_TEST(system::from_string("65535", __uint16_val) == 65535,
             "system::from_string(, uint16_t)");
  uint32_t __uint32_val;
  BOOST_TEST(system::from_string("4294967295", __uint32_val) == 4294967295,
             "system::from_string(, uint32_t)");
  uint64_t __uint64_val;
  BOOST_TEST(system::from_string("9223372036854775807", __uint64_val) ==
                 9223372036854775807,
             "system::from_string(, uint64_t)");

  float __float_val;
  BOOST_TEST(system::from_string("1.0", __float_val) == (float)1,
             "system::from_string(, float)");
  double __double_val;
  BOOST_TEST(system::from_string("1.0", __double_val) == (double)1,
             "system::from_string(, double)");
  long double __ldouble_val;
  BOOST_TEST(system::from_string("1.0", __ldouble_val) == (long double)1,
             "system::from_string(, long double)");
  system::from_string("1", __mpfr);
  BOOST_TEST(mpfr_cmp_ui(__mpfr, 1) == 0);
}
