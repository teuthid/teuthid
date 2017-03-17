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

#include <chrono>
#include <thread>

#include <boost/test/unit_test.hpp>
#include <teuthid/system.hpp>

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
}
