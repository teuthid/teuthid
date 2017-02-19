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

#ifndef TEUTHID_LIBRARY_HPP

#include <string>
#include <teuthid/config.hpp>

namespace teuthid
{

class library
{
public:
  library(library const &) = delete;
  void operator=(library const &) = delete;
  static constexpr int major_version()
  {
    return major_;
  }
  static constexpr int minor_version()
  {
    return minor_;
  }
  static constexpr int patch_version()
  {
    return patch_;
  }
  static constexpr int soversion()
  {
    return soversion_;
  }
  static const std::string &version()
  {
    return version_;
  }
  static bool required_version(int min_major, int min_minor);
  static constexpr bool use_opencl()
  {
    return use_opencl_;
  }

private:
  library() {}
  ~library() {}

private:
  static constexpr int major_ = TEUTHID_MAJOR_VERSION;
  static constexpr int minor_ = TEUTHID_MINOR_VERSION;
  static constexpr int patch_ = TEUTHID_PATCH_VERSION;
  static constexpr int soversion_ = TEUTHID_SOVERSION;
  static const std::string &version_;
#if defined(TEUTHID_USE_OPENCL)
  static constexpr bool use_opencl_ = true;
#else
  static constexpr bool use_opencl_ = false;
#endif
};

} // namespace teuthid

#endif // TEUTHID_LIBRARY_HPP
