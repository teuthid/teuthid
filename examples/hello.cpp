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

#include <iostream>

#include <teuthid/library.hpp>
using namespace teuthid;

#if defined(TEUTHID_USE_ARRAYFIRE)
#define cl_backend "ArrayFire"
#else
#define cl_backend "Boost.Compute"
#endif

#define ruler                                                                  \
  "------------------------------------------------------------------------"

std::string __str;

int main() {
  std::cout << std::endl << ruler << std::endl;
  std::cout << "Teuthid Version " << library::major_version() << "."
            << library::minor_version() << std::endl;
  if (library::have_compute_kernel()) {
    std::cout << "Build with OpenCL (using " << cl_backend << " as a backend).";
    std::cout << std::endl;
    std::cout << "Available OpenCL platform(s): "
              << library::compute_platforms().size() << std::endl;
    for (auto __platform : library::compute_platforms()) {
      std::cout << "  Platform Name: " << __platform.name();
      std::cout << std::endl;
      std::cout << "  Platform Vendor: " << __platform.vendor();
      std::cout << std::endl;
      std::cout << "  Platform Version: " << __platform.version();
      std::cout << "  OpenCL version supported: " << __platform.major_version()
                << "." << __platform.minor_version();
      std::cout << std::endl;
      if (__platform.is_full_profile())
        __str = std::string("FULL PROFILE");
      else if (__platform.is_embedded_profile())
        __str = std::string("EMBEDDED PROFILE");
      else
        __str = std::string("UNKNOWN PROFILE");
      std::cout << "  Platform Profile: " << __str;
      std::cout << std::endl;
      std::cout << "  Platform Extensions: " << __platform.extensions();
      std::cout << std::endl;
      std::cout << "  Platform Host Timer Resolution: "
                << __platform.host_timer_resolution() << std::endl;
      std::cout << "  Platform Extensions function suffix : "
                << __platform.icd_suffix_khr() << std::endl;

      // ... and devices:
      std::cout << "  Available OpenCL devices(s): "
                << __platform.devices().size() << std::endl;
      for (auto __device : __platform.devices()) {
        std::cout << "    Device Name: " << __device.name();
        std::cout << std::endl;
        std::cout << "    Device Version: " << __device.version();
        std::cout << std::endl;
        std::cout << "    Driver Version: " << __device.driver_version();
        std::cout << std::endl;
        std::cout << "    OpenCL C Version: " << __device.c_version();
        std::cout << std::endl;
        std::cout << "    Parallel compute units: "
                  << __device.max_compute_units() << std::endl;
      }
    }
    std::cout << std::endl;
  } else
    std::cout << "Build without OpenCL." << std::endl;

  // ...
  std::cout << std::endl << ruler << std::endl;
  return 0;
}