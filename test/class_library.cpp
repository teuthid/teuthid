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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#define BOOST_TEST_MODULE teuthid
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <teuthid/library.hpp>

using namespace teuthid;

BOOST_AUTO_TEST_CASE(class_teuthid_library)
{
  BOOST_TEST(library::major_version() == TEUTHID_MAJOR_VERSION);
  BOOST_TEST(library::minor_version() == TEUTHID_MINOR_VERSION);
  BOOST_TEST(library::patch_version() == TEUTHID_PATCH_VERSION);
  BOOST_TEST(library::soversion() == TEUTHID_SOVERSION);
  BOOST_TEST(library::version() == std::string(TEUTHID_VERSION));
  
  BOOST_TEST(library::required_version(TEUTHID_MAJOR_VERSION, 
    TEUTHID_MINOR_VERSION));
  BOOST_TEST(!library::required_version(TEUTHID_MAJOR_VERSION + 1, 
    TEUTHID_MINOR_VERSION));
  BOOST_TEST(!library::required_version(TEUTHID_MAJOR_VERSION, 
    TEUTHID_MINOR_VERSION + 1));
  BOOST_TEST(library::required_version(TEUTHID_MAJOR_VERSION - 1, 
    TEUTHID_MINOR_VERSION));
  BOOST_TEST(library::required_version(TEUTHID_MAJOR_VERSION, 
    TEUTHID_MINOR_VERSION - 1));
}

