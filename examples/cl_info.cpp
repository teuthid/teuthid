/*
  This file is part of the Teuthid project.
  Copyright (c) 2016-2017 Mariusz Przygodzki (mariusz.przygodzki@gmail.com)

  The Teuthid is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  The Teuthid is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
  for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the Teuthid; see the file LICENSE.LGPLv3.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <teuthid/system.hpp>
using namespace teuthid;

#if defined(TEUTHID_WITH_OPENCL)
#include <teuthid/clb/platform.hpp>
#endif

std::string __str;

std::string yesno(bool value) {
  return (value ? std::string("YES") : std::string("NO"));
}

int main() {
  std::cout << "Teuthid Version " << system::to_string(system::major_version())
            << "." << system::to_string(system::minor_version()) << std::endl;
  std::cout << "  --" << std::endl;
  if (system::has_cl_backend()) {
#if defined(TEUTHID_WITH_OPENCL)
    std::cout << "Build with OpenCL." << std::endl;

    // OpenCL platforms ...
    std::cout << "Available OpenCL platform(s): " << clb::platform::count()
              << std::endl;
    std::cout << "Default OpenCL platform: "
              << clb::platform::get_default().name() << std::endl;
    std::cout << "Available OpenCL GPU device(s): "
              << clb::device::find_by_type(clb::devtype_t::GPU).size()
              << std::endl;
    std::cout << "Default OpenCL device: " << clb::device::get_default().name()
              << std::endl;

    for (auto __platform : clb::platform::get_all()) {
      std::cout << "  --" << std::endl;
      std::cout << "  Platform Name: " << __platform.name()
                << "  Vendor: " << __platform.vendor() << std::endl;
      std::cout << "  Platform Version: " << __platform.version() << std::endl;
      if (__platform.is_full_profile())
        __str = "FULL";
      else if (__platform.is_embedded_profile())
        __str = "EMBEDDED";
      std::cout << "  Platform Profile: " << __str << std::endl;
      std::cout << "  Platform Extensions: "
                << system::to_string(__platform.extensions()) << std::endl;
      std::cout << "  Platform Extensions function suffix : "
                << __platform.icd_suffix_khr() << std::endl;
      // ... and devices:
      std::cout << "  Available OpenCL device(s): " << __platform.device_count()
                << std::endl;

      for (auto __device : __platform.devices()) {
        std::cout << "    Device Name: " << __device.name() << std::endl;
        std::cout << "    Device Version: " << __device.version()
                  << "    Driver Version: " << __device.driver_version()
                  << std::endl;
        std::cout << "    OpenCL C Version: " << __device.c_version()
                  << std::endl;
        if (__device.is_full_profile())
          __str = "FULL";
        else if (__device.is_embedded_profile())
          __str = "EMBEDDED";
        std::cout << "    Device Profile: " << __str << std::endl;
        if (__device.is_cpu())
          __str = "CPU";
        else if (__device.is_gpu())
          __str = "GPU";
        std::cout << "    Device Type: " << __str
                  << "    Compiler: " << yesno(__device.is_compiler_available())
                  << std::endl;
        std::cout << "    Max compute units: " << __device.max_compute_units();
        std::cout << "    Max subdevices: "
                  << system::to_string(__device.max_subdevices()) << std::endl;
        std::cout << "    Precision:  double="
                  << yesno(__device.has_double_precision())
                  << "  single=" << yesno(__device.has_single_precision());
        std::cout << std::endl;
        std::cout << "    Device Extensions: "
                  << system::to_string(__device.extensions());
        std::cout << std::endl;
        std::cout << "    Built-in kernels: "
                  << system::to_string(__device.built_in_kernels());
        std::cout << std::endl;
        std::cout << "    Address bits: "
                  << system::to_string(__device.address_bits())
                  << "    Max alloc size: "
                  << system::to_string(__device.max_mem_alloc_size());
        std::cout << std::endl;
        std::cout << "    Global memory: "
                  << system::to_string(__device.global_mem_size()) << "  ";
        std::cout << "    Local memory: "
                  << system::to_string(__device.local_mem_size()) << std::endl;
        std::cout << "    Global memory cache: "
                  << system::to_string(__device.global_mem_cache_size());
        std::cout << "    Global memory cacheline: "
                  << system::to_string(__device.global_mem_cacheline_size());
        std::cout << std::endl;
        std::cout
            << "    Native vector width:  char="
            << system::to_string(__device.native_vector_width<int8_t>())
            << "  short="
            << system::to_string(__device.native_vector_width<int16_t>())
            << "  int="
            << system::to_string(__device.native_vector_width<int32_t>())
            << "  long="
            << system::to_string(__device.native_vector_width<int64_t>())
            << "  half="
            << system::to_string(__device.native_vector_width<float16_t>())
            << "  float="
            << system::to_string(__device.native_vector_width<float32_t>())
            << "  double="
            << system::to_string(__device.native_vector_width<float64_t>());
        std::cout << std::endl;
        std::cout
            << "    Preferred vector width:  char="
            << system::to_string(__device.preferred_vector_width<int8_t>())
            << "  short="
            << system::to_string(__device.preferred_vector_width<int16_t>())
            << "  int="
            << system::to_string(__device.preferred_vector_width<int32_t>())
            << "  long="
            << system::to_string(__device.preferred_vector_width<int64_t>())
            << "  half="
            << system::to_string(__device.preferred_vector_width<float16_t>())
            << "  float="
            << system::to_string(__device.preferred_vector_width<float32_t>())
            << "  double="
            << system::to_string(__device.preferred_vector_width<float64_t>());
        std::cout << std::endl;
      }
    }
#endif // TEUTHID_WITH_OPENCL
  } else
    std::cout << "Build without OpenCL." << std::endl;
  std::cout << std::endl;
  return 0;
}