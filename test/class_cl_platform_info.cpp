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
#include <teuthid/cl_platform_info.hpp>
#include <teuthid/library.hpp>

using namespace teuthid;
using namespace teuthid::cl;

BOOST_AUTO_TEST_CASE(class_teuthid_cl_platform_info) {
  platform_info __info;
  BOOST_TEST(!__info.id(), "cl::platform_info.id()");
  BOOST_TEST(__info.profile() == UNKNOWN_PROFILE,
             "cl::platform_info.profile()");
  BOOST_TEST(__info.version().empty(), "cl::platform_info.version()");
  BOOST_TEST(__info.major_version() == 0, "cl::platform_info.major_version()");
  BOOST_TEST(__info.minor_version() == 0, "cl::platform_info.minor_version()");
  BOOST_TEST(__info.spec_version().empty(), "cl::platform_info.spec_version()");
  BOOST_TEST(__info.name().empty(), "cl::platform_info.name()");
  BOOST_TEST(__info.vendor().empty(), "cl::platform_info.vendor()");
  BOOST_TEST(__info.extensions().empty(), "cl::platform_info.extensions()");
  BOOST_TEST(__info.host_timer_resolution() == 0,
             "cl::platform_info.host_timer_resolution()");
  BOOST_TEST(__info.icd_suffix_khr().empty(), "cl::icd_suffix_khr.vendor()");
  opencl_platforms_t __platforms = library::opencl_platforms();
#if defined(TEUTHID_WITH_OPENCL)
  BOOST_TEST(!__platforms.empty());
  for (auto __platform : __platforms) {
    BOOST_TEST(__platform.id(), "cl::platform_info.id()");
    BOOST_TEST(__platform.profile() != UNKNOWN_PROFILE,
               "cl::platform_info.profile()");
    BOOST_TEST(
        (__platform.is_full_profile() || __platform.is_embedded_profile()),
        "cl::platform_info.is_full_profile()");
    BOOST_TEST(!__platform.version().empty(), "cl::platform_info.version()");
    BOOST_TEST(__platform.major_version() > 0,
               "cl::platform_info.major_version()");
    BOOST_TEST(!__platform.spec_version().empty(),
               "cl::platform_info.spec_version()");
    BOOST_TEST(!__platform.name().empty(), "cl::platform_info.name()");
    BOOST_TEST(!__platform.vendor().empty(), "cl::platform_info.vendor()");
    BOOST_TEST(__platform.is_required_version(__platform.major_version(),
                                              __platform.minor_version()),
               "cl::platform_info.is_required_version()");
    BOOST_TEST(__platform.is_required_version(0, 0),
               "cl::platform_info.is_required_version()");
    BOOST_TEST(
        !__platform.is_required_version(__platform.major_version() + 1, 0),
        "cl::platform_info.is_required_version()");
    BOOST_TEST(!__platform.is_required_version(__platform.major_version(),
                                               __platform.minor_version() + 1),
               "cl::platform_info.is_required_version()");
    __info = __platform;
    BOOST_TEST(__info.id(), "cl::platform_info.id()");
  }
#else
  BOOST_TEST(__platforms.empty());
#endif // defined(TEUTHID_WITH_OPENCL)
}
