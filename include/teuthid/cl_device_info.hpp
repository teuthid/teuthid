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

#ifndef TEUTHID_CL_DEVICE_INFO_HPP
#define TEUTHID_CL_DEVICE_INFO_HPP

#include <map>
#include <mutex>
#include <string>

#include <teuthid/cl_platform_info.hpp>

#if defined(TEUTHID_WITH_OPENCL)
#if defined(__APPLE__)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#endif

namespace teuthid {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(TEUTHID_WITH_OPENCL)
typedef cl_device_id opencl_device_id_t;
#else
typedef int *opencl_device_id_t;
#endif // defined(TEUTHID_WITH_OPENCL)
#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace cl {
class device_info;
}
typedef std::multimap<opencl_platform_id_t, cl::device_info> opencl_devices_t;

namespace cl {

class device_info {
public:
  device_info() {}
  ~device_info() {}

  static const opencl_devices_t &devices(const platform_info &platform,
                                         bool force_detection = false);

private:
  static std::mutex mutex_;
  static bool devices_detected_;
  static opencl_devices_t devices_;
};

} // namespace cl
} // namespace teuthid

#endif // TEUTHID_CL_DEVICE_INFO_HPP