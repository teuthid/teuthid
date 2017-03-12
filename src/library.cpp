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

#include <teuthid/library.hpp>

using namespace teuthid;

thread_local bool library::use_compute_kernel_ = library::have_compute_kernel();
std::string library::version_ = std::string(TEUTHID_VERSION);

bool library::is_required_version(uint8_t major, uint8_t minor) noexcept {
  uint32_t __required = major * 1000 + minor;
  uint32_t __actual = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(__required > __actual));
}

bool library::have_compute_kernel() {
  for (auto __platform : compute_platform::platforms())
    if (__platform.devices().size() > 0)
      return true;
  return false;
}

bool library::use_compute_kernel(bool enabled) {
  if (library::have_compute_kernel())
    library::use_compute_kernel_ = enabled;
  return library::use_compute_kernel_;
}
