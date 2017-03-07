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

#include <cassert>

#include <teuthid/cl_device_info.hpp>

using namespace teuthid;
using namespace teuthid::cl;

std::mutex device_info::mutex_;
bool device_info::devices_detected_ = false;
opencl_devices_t device_info::devices_ = opencl_devices_t();

const opencl_devices_t &device_info::devices(const platform_info &platform,
                                             bool force_detection) {}
