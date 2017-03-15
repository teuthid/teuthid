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
  compute_platform __empty;
  BOOST_TEST(!__empty.id(), "id()");
  BOOST_TEST(__empty.profile() == COMPUTE_UNKNOWN_PROFILE, "profile()");
  BOOST_TEST(__empty.version().empty(), "version()");
  BOOST_TEST(__empty.major_version() == 0, "major_version()");
  BOOST_TEST(__empty.minor_version() == 0, "minor_version()");
  BOOST_TEST(__empty.spec_version().empty(), "spec_version()");
  BOOST_TEST(__empty.name().empty(), "name()");
  BOOST_TEST(__empty.vendor().empty(), "vendor()");
  BOOST_TEST(__empty.extensions().empty(), "extensions()");
  BOOST_TEST(!__empty.have_extension(""), "have_extension()");
  BOOST_TEST(!__empty.have_extension("xxx"), "have_extension()");
  BOOST_TEST(__empty.icd_suffix_khr().empty(), "icd_suffix_khr.vendor()");
  BOOST_TEST(__empty.devices().empty(), "devices()");
  BOOST_TEST(__empty.num_devices() == 0, "num_devices()");

  compute_platform_id_t *fake_id_ptr_ = new compute_platform_id_t;
  __empty = compute_platform(*fake_id_ptr_);
  BOOST_TEST(__empty.profile() == COMPUTE_UNKNOWN_PROFILE, "profile()");
  BOOST_TEST(__empty.version().empty(), "version()");

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
    BOOST_TEST(__platform.num_devices() > 0, "num_devices()");
    compute_platform __p2 = __platform;
    BOOST_TEST(__p2.id(), "id()");
    __p2 = compute_platform(__platform.id());
    BOOST_TEST(__p2.id(), "id()");
    BOOST_TEST(__p2.profile() != COMPUTE_UNKNOWN_PROFILE, "profile()");
    BOOST_TEST(!__p2.version().empty(), "version()");
  }
#else
  BOOST_TEST(__platforms.empty());
#endif // defined(TEUTHID_WITH_OPENCL)
}
