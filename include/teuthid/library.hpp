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

#ifndef TEUTHID_LIBRARY_HPP

#include <string>

#include <teuthid/cl_info.hpp>

namespace teuthid {

class library final {
public:
  library(library const &) = delete;
  void operator=(library const &) = delete;
  static constexpr int major_version() { return TEUTHID_MAJOR_VERSION; }
  static constexpr int minor_version() { return TEUTHID_MINOR_VERSION; }
  static constexpr int patch_version() { return TEUTHID_PATCH_VERSION; }
  static constexpr int soversion() { return TEUTHID_SOVERSION; }
  static const std::string &version() { return library::version_; }
  static bool is_required_version(int min_major, int min_minor);
  static const opencl_platforms_t &opencl_platforms() {
    return cl::platform_info::platforms();
  }
  static bool have_opencl() { return (library::opencl_platforms().size() > 0); }
  static bool use_opencl() { return library::use_opencl_; }
  static bool use_opencl(bool enabled);

private:
  library() {}
  ~library() {}

  static std::string version_;
  static thread_local bool use_opencl_;
}; // class library
} // namespace teuthid

#endif // TEUTHID_LIBRARY_HPP
