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
  std::cout << "Teuthid Version " << std::to_string(library::major_version())
            << "." << std::to_string(library::minor_version()) << std::endl;
  std::cout << "  --" << std::endl;
  if (library::have_compute_kernel()) {
    std::cout << "Build with OpenCL (using " << cl_backend << " as a backend)."
              << std::endl;
    std::cout << "Available OpenCL platform(s): "
              << library::compute_platforms().size() << std::endl;
    for (auto __platform : library::compute_platforms()) {
      std::cout << "  Platform Name: " << __platform.name() << std::endl;
      std::cout << "  Platform Vendor: " << __platform.vendor() << std::endl;
      std::cout << "  Platform Version: " << __platform.version() << std::endl;
      std::cout << "    OpenCL supported: " << __platform.major_version() << "."
                << __platform.minor_version();
      std::cout << "  Specific: " << __platform.spec_version() << std::endl;
      if (__platform.is_full_profile())
        __str = "FULL PROFILE";
      else if (__platform.is_embedded_profile())
        __str = "EMBEDDED PROFILE";
      else
        __str = "UNKNOWN PROFILE";
      std::cout << "  Platform Profile: " << __str << std::endl;
      std::cout << "  Platform Extensions: ";
      for (auto __ext : __platform.extensions())
        std::cout << __ext << "  ";
      std::cout << std::endl;
      std::cout << "  Platform Extensions function suffix : "
                << __platform.icd_suffix_khr() << std::endl;

      // ... and devices:
      std::cout << "  Available OpenCL devices(s): " << __platform.num_devices()
                << std::endl;
      for (auto __device : __platform.devices()) {
        std::cout << "    Device Name: " << __device.name() << std::endl;
        std::cout << "    Device Version: " << __device.version() << std::endl;
        std::cout << "    Driver Version: " << __device.driver_version()
                  << std::endl;
        std::cout << "    OpenCL C Version: " << __device.c_version()
                  << std::endl;
        if (__device.is_full_profile())
          __str = "FULL PROFILE";
        else if (__device.is_embedded_profile())
          __str = "EMBEDDED PROFILE";
        else
          __str = "UNKNOWN PROFILE";
        std::cout << "    Device Profile: " << __str << std::endl;
        if (__device.is_devtype_cpu())
          __str = "CPU";
        else if (__device.is_devtype_gpu())
          __str = "GPU";
        else if (__device.is_devtype_accelerator())
          __str = "ACCELERATOR";
        else
          __str = "UNKNOWN PROFILE";
        std::cout << "    Device Type: " << __str << std::endl;
        std::cout << "    Parallel compute units: "
                  << __device.max_compute_units() << std::endl;
        std::cout << "    Device Extensions: " << __device.extensions()
                  << std::endl;
      }
    }
    std::cout << std::endl;
  } else
    std::cout << "Build without OpenCL." << std::endl;

  // ...
  std::cout << std::endl << ruler << std::endl;
  return 0;
}