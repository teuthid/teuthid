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

#if defined(TEUTHID_USE_BOOST_COMPUTE)
#define opencl_backend "Boost.Compute"
#elif defined(TEUTHID_USE_ARRAYFIRE)
#define opencl_backend "ArrayFire"
#endif

#define ruler                                                                  \
  "------------------------------------------------------------------------"

std::string __str;

int main() {
  std::cout << std::endl << ruler << std::endl;
  std::cout << "Teuthid Version " << library::version() << "." << std::endl;
  if (library::have_opencl()) {
    std::cout << "Build with OpenCL (using " << opencl_backend << ").";
    std::cout << std::endl;
    std::cout << "Available OpenCL platform(s) and device(s):" << std::endl;
    for (auto __platform : library::opencl_platforms()) {
      if (__platform.profile() == cl::platform_info::FULL_PROFILE)
        __str = std::string("FULL_PROFILE");
      std::cout << "  Platform profile:  " << __str;
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  // ...
  std::cout << std::endl << ruler << std::endl;
  return 0;
}