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

#include <cassert>
#include <teuthid/library.hpp>

using namespace teuthid;

thread_local bool library::use_opencl_ = library::has_opencl();
library::threads_map_t_ library::cl_in_threads_ = {
    {std::this_thread::get_id(), library::has_opencl()}};
std::mutex library::mutex_;
std::string library::version_ = std::string(TEUTHID_VERSION);

const std::string &library::version() { return library::version_; }

bool library::is_required_version(int min_major, int min_minor) {
  int __required = min_major * 1000 + min_minor;
  int __actual = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(__required > __actual));
}

inline bool library::has_opencl() {
#if defined(TEUTHID_HAS_OPENCL)
  return true;
#else
  return false;
#endif
}

bool library::use_opencl() {
  assert(!(library::cl_in_threads_).empty());
  return library::cl_in_threads_[std::this_thread::get_id()];
  // return library::use_opencl_
}

bool library::use_opencl(bool enabled) {
  assert(!(library::cl_in_threads_).empty());
  std::lock_guard<std::mutex> __guard(library::mutex_);
  if (library::has_opencl())
    library::cl_in_threads_[std::this_thread::get_id()] = enabled;
  return library::use_opencl();
  // if (library::has_opencl())
  //   library::use_opencl_ = enabled;
  // return library::use_opencl();
}
