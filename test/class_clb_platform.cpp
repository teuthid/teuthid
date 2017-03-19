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

BOOST_AUTO_TEST_CASE(class_teuthid_clb_platform) {
  const platforms_t &__platforms = platform::platforms();
  BOOST_TEST(!__platforms.empty());
  BOOST_TEST(platform::platform_count() > 0);

  for (auto __platform : __platforms) {
    BOOST_TEST(__platform.id(), "id()");
    BOOST_TEST(__platform.profile() != UNKNOWN_PROFILE, "profile()");
    BOOST_TEST(
        (__platform.is_full_profile() || __platform.is_embedded_profile()),
        "is_full_profile(), is_embedded_profile()");
    BOOST_TEST(!__platform.version().empty(), "version()");
    BOOST_TEST(__platform.major_version() > 0, "major_version()");
    BOOST_TEST(!__platform.spec_version().empty(), "spec_version()");
    BOOST_TEST(!__platform.name().empty(), "name()");
    BOOST_TEST(!__platform.vendor().empty(), "vendor()");
    for (auto __ext : __platform.extensions()) {
      BOOST_TEST(!__ext.empty(), "extensions()");
      BOOST_TEST(__platform.have_extension(__ext), "have_extension()");
    }
    BOOST_TEST(!__platform.have_extension(" "), "have_extension()");
    BOOST_TEST(!__platform.have_extension("xxx"), "have_extension()");
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
    BOOST_TEST(__platform.device_count() > 0, "device_count()");
    
    platform __plat = __platform;
    BOOST_TEST((__plat == __platform));
    BOOST_TEST(!(__plat != __platform));
    BOOST_TEST(__plat.id(), "id()");
    __plat = platform(__platform.id());
    BOOST_TEST(__plat.id(), "id()");
    BOOST_TEST(__plat.profile() != UNKNOWN_PROFILE, "profile()");
    BOOST_TEST(!__plat.version().empty(), "version()");
  }
}
