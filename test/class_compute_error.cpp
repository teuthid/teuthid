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
#include <teuthid/compute_error.hpp>

using namespace teuthid;

bool is_critical(compute_error const &) { return true; }

void some_compute_error() { throw compute_error("some_compute_error"); }

void some_invalid_compute_platform() {
  throw invalid_compute_platform("some_invalid_compute_platform");
}

void some_invalid_compute_device() {
  throw invalid_compute_device("some_invalid_compute_device");
}

BOOST_AUTO_TEST_CASE(class_teuthid_compute_error) {
  BOOST_CHECK_EXCEPTION(some_compute_error(), compute_error, is_critical);
  BOOST_CHECK_EXCEPTION(some_invalid_compute_platform(),
                        invalid_compute_platform, is_critical);
  BOOST_CHECK_EXCEPTION(some_invalid_compute_device(),
                        invalid_compute_device, is_critical);
}