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

#include <teuthid/system.hpp>

using namespace teuthid;

#if defined(TEUTHID_WITH_OPENCL)
thread_local bool system::use_clb_ = system::have_clb();
#else
thread_local bool system::use_clb_ = false;
#endif

std::string system::version_ = std::string(TEUTHID_VERSION);

bool system::is_required_version(uint8_t major, uint8_t minor) noexcept {
  uint32_t __required = major * 1000 + minor;
  uint32_t __actual = TEUTHID_MAJOR_VERSION * 1000 + TEUTHID_MINOR_VERSION;
  return (!(__required > __actual));
}

bool system::have_clb() {
#if defined(TEUTHID_WITH_OPENCL)
  try {
    for (auto __platform : compute_platform::platforms())
      if (__platform.devices().size() > 0)
        return true;
  } catch (const compute_error &) {
    // some problems with the compute kernel - it wil be disabled
  }
#endif // TEUTHID_WITH_OPENCL
  return false;
}

bool system::use_clb(bool enabled) {
  if (system::have_clb())
    system::use_clb_ = enabled;
  return system::use_clb_;
}
