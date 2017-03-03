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

#include <teuthid/cl_info.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#endif

using namespace teuthid;
using namespace teuthid::cl;

bool platform_info::platforms_detected_ = false;
opencl_platforms_t platform_info::platforms_ = opencl_platforms_t();

const opencl_platforms_t &platform_info::platforms(bool force_detection) {
  if (!platform_info::platforms_detected_ || force_detection) {
    platform_info::detect_platforms_();
    platform_info::platforms_detected_ = true;
  }
  return platform_info::platforms_;
}

void platform_info::detect_platforms_() {
  platform_info::platforms_.clear();
#if defined(TEUTHID_WITH_OPENCL)

// ...
#endif
}