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

#include <algorithm>
#include <cassert>

#include <teuthid/clb/error.hpp>
#include <teuthid/clb/platform.hpp>

using namespace teuthid::clb;

device::device(device_id_t device_id) {
  assert(device_id);
  bool __found = false;
  try {
    const platform &__platform = device::get_platform(device_id);
    for (const device &__device : __platform.devices())
      if (device_id == __device.id()) {
        *this = device(__device);
        __found = true;
      }
  } catch (const error &__e) {
    throw invalid_device(__e.cl_error());
  }
  if (!__found)
    throw invalid_device("unknown device_id_t");
}

const platform &device::get_platform() const {
  try {
    const platforms_t &__platforms = platform::platforms();
    for (std::size_t __i = 0; __i < __platforms.size(); __i++)
      if (platform_id_ == __platforms[__i].id())
        return __platforms[__i];
  } catch (const error &__e) {
    throw invalid_platform(__e.cl_error());
  }
  throw invalid_platform("cannot find a platform");
}

bool device::have_extension(const std::string &ext_name) const {
  if (!ext_name.empty())
    return std::find(device::extensions_.begin(), device::extensions_.end(),
                     ext_name) != device::extensions_.end();
  return false;
}

const platform &device::get_platform(device_id_t device_id) {
  try {
    const platforms_t &__platforms = platform::platforms();
    for (std::size_t __i = 0; __i < __platforms.size(); __i++) {
      const devices_t &__devices = __platforms[__i].devices();
      for (std::size_t __j = 0; __j < __devices.size(); __j++)
        if (device_id == __devices[__j].id())
          return __platforms[__i];
    }
  } catch (const error &__e) {
    throw invalid_platform(__e.cl_error());
  }
  throw invalid_platform("cannot find a platform");
}
