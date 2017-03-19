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
#include <teuthid/clb/platform.hpp>

using namespace teuthid::clb;

BOOST_AUTO_TEST_CASE(class_teuthid_clb_device) {
  const platforms_t &__platforms = platform::platforms();
  BOOST_TEST(!__platforms.empty());

  for (auto __platform : __platforms) {
    auto __devices = __platform.devices();
    for (auto __device : __devices) {
      BOOST_TEST(__device.id(), "id()");
      BOOST_TEST(__device.profile() != UNKNOWN_PROFILE, "profile()");
      BOOST_TEST(__device.devtype() != DEVICE_UNKNOWN, "devtype()");
      BOOST_TEST(!__device.name().empty(), "name()");
      BOOST_TEST(!__device.version().empty(), "version()");
      BOOST_TEST(!__device.driver_version().empty(), "driver_version()");
      BOOST_TEST(!__device.c_version().empty(), "c_version()");
      BOOST_TEST(__device.max_compute_units() > 0, "max_compute_units()");
      BOOST_TEST(!__device.extensions().empty(), "extensions()");
      for (auto __ext : __device.extensions())
        BOOST_TEST(!__ext.empty(), "extensions()");
      BOOST_TEST(__device.get_platform().id() == __platform.id(),
                 "get_platform()");
      BOOST_TEST((device::get_platform(__device.id()) == __platform),
                 "device::get_platform()");
      device __dev = __device;
      BOOST_TEST(__dev.id(), "id()");
      BOOST_TEST(__dev.id() == __device.id(), "id()");
      __dev = device(__device.id());
      BOOST_TEST(__dev.id(), "id()");
      BOOST_TEST(__dev.id() == __device.id(), "id()");
      BOOST_TEST(__dev.get_platform().id() == __platform.id(),
                 "get_platform()");
    }
  }
}