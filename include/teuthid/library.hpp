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
#define TEUTHID_LIBRARY_HPP

#include <string>

#include <teuthid/compute_platform.hpp>

namespace teuthid {

class library final {
public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  library(library const &) = delete;
  void operator=(library const &) = delete;
#endif // DOXYGEN_SHOULD_SKIP_THIS
  static constexpr uint8_t major_version() noexcept {
    return TEUTHID_MAJOR_VERSION;
  }
  static constexpr uint8_t minor_version() noexcept {
    return TEUTHID_MINOR_VERSION;
  }
  static constexpr uint8_t patch_version() noexcept {
    return TEUTHID_PATCH_VERSION;
  }
  static constexpr uint8_t soversion() noexcept { return TEUTHID_SOVERSION; }
  static const std::string &version() noexcept { return library::version_; }
  static bool is_required_version(uint8_t major, uint8_t minor) noexcept;
  static const compute_platforms_t &compute_platforms() {
    return compute_platform::platforms();
  }
  static bool have_compute_kernel();
  static bool use_compute_kernel() { return library::use_compute_kernel_; }
  static bool use_compute_kernel(bool enabled);

private:
  library() {}
  ~library() {}

  static std::string version_;
  static thread_local bool use_compute_kernel_;
}; // class library
} // namespace teuthid

#endif // TEUTHID_LIBRARY_HPP
