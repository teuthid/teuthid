#define CL_HPP_ENABLE_EXCEPTIONS
// passed byc CMake: #define CL_HPP_TARGET_OPENCL_VERSION 200
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
// #define CL_HPP_CL_1_2_DEFAULT_BUILD

#if defined(__APPLE__)
#include <OpenCL/cl2.hpp>
#include <AvailabilityMacros.h>
#else
#include <CL/cl2.hpp>
#endif

#include <vector>

int main(void)
{
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  cl::Platform plat;
  for (auto &p : platforms)
  {
    std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
    if (platver.find("OpenCL 2.") != std::string::npos)
    {
      plat = p;
    }
  }
  if (plat() == 0)
    return -1;

  cl::Platform newP = cl::Platform::setDefault(plat);
  if (newP != plat)
    return -1;

  cl::DeviceCommandQueue defaultDeviceQueue =
      cl::DeviceCommandQueue::makeDefault(cl::Context::getDefault(),
                                          cl::Device::getDefault());
  return 0;
}
