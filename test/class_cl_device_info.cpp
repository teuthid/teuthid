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
#include <teuthid/library.hpp>

#include <teuthid/cl_device_info.hpp>
#include <teuthid/cl_platform_info.hpp>

using namespace teuthid;
using namespace teuthid::cl;

BOOST_AUTO_TEST_CASE(class_teuthid_cl_device_info) {
  device_info __info;
  BOOST_TEST(!__info.id(), "cl::device_info.id()");
  BOOST_TEST(__info.name().empty(), "cl::device_info.name()");
  BOOST_TEST(__info.version().empty(), "cl::device_info.version()");
  BOOST_TEST(__info.driver_version().empty(),
             "cl::device_info.driver_version()");
  BOOST_TEST(__info.c_version().empty(), "cl::device_info.c_version()");
  BOOST_TEST(__info.max_compute_units() == 0,
             "cl::device_info.max_compute_units()");
  opencl_platforms_t __platforms = library::opencl_platforms();
  opencl_devices_t __devices;
#if defined(TEUTHID_WITH_OPENCL)
  BOOST_TEST(!__platforms.empty());
  for (auto __platform : __platforms) {
    __devices = __platform.devices();
    for (auto __device : __devices) {
      BOOST_TEST(__device.id(), "cl::device_info.id()");
      BOOST_TEST(!__device.name().empty(), "cl::device_info.name()");
      BOOST_TEST(!__device.version().empty(), "cl::device_info.version()");
      BOOST_TEST(!__device.driver_version().empty(),
                 "cl::device_info.driver_version()");
      BOOST_TEST(!__device.c_version().empty(), "cl::device_info.c_version()");
      BOOST_TEST(__device.max_compute_units() > 0,
                 "cl::device_info.max_compute_units()");
      __info = __device;
      BOOST_TEST(__info.id(), "cl::platform_info.id()");
    }
  }
#else
  BOOST_TEST(__devices.empty());
#endif // defined(TEUTHID_WITH_OPENCL)
}