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
  void *__void = reinterpret_cast<void *>(13);
  BOOST_TEST((system::to_string(__void)) == "13", "to_string(void*)");

  mpfr_t __mpfr;
  mpfr_init_set_ld(__mpfr, 1.2345, mpfr_get_default_rounding_mode());
  BOOST_TEST(!system::to_string(__mpfr).empty());
  system::format_float_output(10);
  BOOST_TEST((system::to_string(__mpfr) == "1.2345000000"),
             "system::format_float_output(10, false)");
  system::format_float_output(10, true);
  BOOST_TEST((system::to_string(__mpfr) == "1.2345000000e+00"),
             "system::format_float_output(10, true)");

  std::string __bool_str = "F A L S E";
  bool __boolval;
  system::from_string(__bool_str, __boolval);
  BOOST_TEST(!__boolval, "system::from_string(, bool)");
  __bool_str = "true";
  system::from_string(__bool_str, __boolval);
  BOOST_TEST(__boolval, "system::from_string(, bool)");
  __bool_str = "";
  BOOST_TEST(!system::from_string(__bool_str, __boolval),
             "system::from_string()");
}
