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

#include <mutex>
#include <string>
#include <teuthid/config.hpp>
#include <thread>
#include <unordered_map>

namespace teuthid {

class library final {
public:
  library(library const &) = delete;
  void operator=(library const &) = delete;
  static constexpr int major_version() { return TEUTHID_MAJOR_VERSION; }
  static constexpr int minor_version() { return TEUTHID_MINOR_VERSION; }
  static constexpr int patch_version() { return TEUTHID_PATCH_VERSION; }
  static constexpr int soversion() { return TEUTHID_SOVERSION; }
  static const std::string &version();
  static bool is_required_version(int min_major, int min_minor);
  static bool has_opencl();
  static bool use_opencl();
  static bool use_opencl(bool force_use);

private:
  typedef std::unordered_map<std::thread::id, bool> threads_map_t_;

  library() {}
  ~library() {}

  static std::string version_;
  static std::mutex mutex_;
  static threads_map_t_ cl_in_threads_;
}; // class library

} // namespace teuthid

#endif // TEUTHID_LIBRARY_HPP
