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

#if defined(TEUTHID_WITH_OPENCL)
bool library::have_opencl_ = true;
#else
bool library::have_opencl_ = false;
#endif

thread_local bool library::use_opencl_ = library::have_opencl_;
std::mutex library::mutex_;
std::string library::version_ = std::string(TEUTHID_VERSION);

bool library::is_required_version(int min_major, int min_minor) {
  int __required = min_major * 1000 + min_minor;
  int __actual = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(__required > __actual));
}

bool library::use_opencl(bool enabled) {
  if (library::have_opencl_)
    library::use_opencl_ = enabled;
  return library::use_opencl_;
}
