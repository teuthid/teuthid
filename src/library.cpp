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

#include <teuthid/library.hpp>

using namespace teuthid;
namespace serialization = boost::serialization;

library::library()
{
#if defined(TEUTHID_HAS_OPENCL)
  use_opencl_ = true;
#else
  use_opencl_ = false;
#endif
}

bool library::is_required_version(int min_major, int min_minor)
{
  int required_ = min_major * 1000 + min_minor;
  int actual_ = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(required_ > actual_));
}

bool library::use_opencl()
{
  return (serialization::singleton<library>::get_const_instance()).use_opencl_;
}

bool library::use_opencl(bool use_)
{
#if defined(TEUTHID_HAS_OPENCL)
  serialization::singleton_module::lock();
  // TO DO
  serialization::singleton_module::unlock();
#endif
  return use_opencl();
}
