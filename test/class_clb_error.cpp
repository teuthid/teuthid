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

#define BOOST_TEST_MODULE teuthid_clb
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <teuthid/clb/error.hpp>

#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

using namespace teuthid::clb;

bool is_critical(error const &) { return true; }
void some_error() { throw error("some_error"); }
void some_invalid_platform() {
  throw invalid_platform("some_invalid_platform");
}
void some_invalid_platform(int error) { throw invalid_platform(error); }
void some_invalid_device() { throw invalid_device("some_invalid_device"); }
void some_invalid_device(int error) { throw invalid_device(error); }

BOOST_AUTO_TEST_CASE(class_teuthid_clb_error) {
  BOOST_CHECK_EXCEPTION(some_error(), error, is_critical);
  BOOST_CHECK_EXCEPTION(some_invalid_platform(), invalid_platform, is_critical);
  BOOST_CHECK_EXCEPTION(some_invalid_platform(CL_INVALID_PLATFORM),
                        invalid_platform, is_critical);
  BOOST_CHECK_EXCEPTION(some_invalid_device(), invalid_device, is_critical);
  BOOST_CHECK_EXCEPTION(some_invalid_device(CL_INVALID_DEVICE), invalid_device,
                        is_critical);
  try {
    some_invalid_platform(CL_INVALID_PLATFORM);
  } catch (const error &__e) {
    BOOST_TEST(__e.cl_error() == CL_INVALID_PLATFORM, "cl_error()");
    BOOST_TEST(std::string(__e.what()) == "CL_INVALID_PLATFORM", "what()");
  }
  try {
    some_invalid_device(CL_INVALID_DEVICE);
  } catch (const invalid_device &__e) {
    BOOST_TEST(__e.cl_error() == CL_INVALID_DEVICE, "cl_error()");
    BOOST_TEST(std::string(__e.what()) == "CL_INVALID_DEVICE", "what()");
  }
}
