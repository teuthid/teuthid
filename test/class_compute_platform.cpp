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
#include <teuthid/compute_platform.hpp>
#include <teuthid/library.hpp>

using namespace teuthid;

BOOST_AUTO_TEST_CASE(class_teuthid_compute_platform) {
  compute_platform __info;
  BOOST_TEST(!__info.id(), "id()");
  BOOST_TEST(__info.profile() == COMPUTE_UNKNOWN_PROFILE, "profile()");
  BOOST_TEST(__info.version().empty(), "version()");
  BOOST_TEST(__info.major_version() == 0, "major_version()");
  BOOST_TEST(__info.minor_version() == 0, "minor_version()");
  BOOST_TEST(__info.spec_version().empty(), "spec_version()");
  BOOST_TEST(__info.name().empty(), "name()");
  BOOST_TEST(__info.vendor().empty(), "vendor()");
  BOOST_TEST(__info.extensions().empty(), "extensions()");
  BOOST_TEST(__info.icd_suffix_khr().empty(), "icd_suffix_khr.vendor()");
  BOOST_TEST(__info.devices().empty(), "devices()");
  BOOST_TEST(__info.num_devices() == 0, "num_devices()");
  compute_platforms_t __platforms = library::compute_platforms();
#if defined(TEUTHID_WITH_OPENCL)
  BOOST_TEST(!__platforms.empty());
  for (auto __platform : __platforms) {
    BOOST_TEST(__platform.id(), "id()");
    BOOST_TEST(__platform.profile() != COMPUTE_UNKNOWN_PROFILE, "profile()");
    BOOST_TEST(
        (__platform.is_full_profile() || __platform.is_embedded_profile()),
        "is_full_profile(), is_embedded_profile()");
    BOOST_TEST(!__platform.version().empty(), "version()");
    BOOST_TEST(__platform.major_version() > 0, "major_version()");
    BOOST_TEST(!__platform.spec_version().empty(), "spec_version()");
    BOOST_TEST(!__platform.name().empty(), "name()");
    BOOST_TEST(!__platform.vendor().empty(), "vendor()");
    BOOST_TEST(__platform.is_required_version(__platform.major_version(),
                                              __platform.minor_version()),
               "is_required_version()");
    BOOST_TEST(__platform.is_required_version(0, 0), "is_required_version()");
    BOOST_TEST(
        !__platform.is_required_version(__platform.major_version() + 1, 0),
        "is_required_version()");
    BOOST_TEST(!__platform.is_required_version(__platform.major_version(),
                                               __platform.minor_version() + 1),
               "is_required_version()");
    BOOST_TEST(!__platform.devices().empty(), "devices()");
    BOOST_TEST(__platform.num_devices() > 0, "num_devices()");
    __info = __platform;
    BOOST_TEST(__info.id(), "id()");
  }
#else
  BOOST_TEST(__platforms.empty());
#endif // defined(TEUTHID_WITH_OPENCL)
}
