/*******************************************************************************
    2  * Copyright (c) 2008-2016 The Khronos Group Inc.
    3  *
    4  * Permission is hereby granted, free of charge, to any person obtaining a
    5  * copy of this software and/or associated documentation files (the
    6  * "Materials"), to deal in the Materials without restriction, including
    7  * without limitation the rights to use, copy, modify, merge, publish,
    8  * distribute, sublicense, and/or sell copies of the Materials, and to
    9  * permit persons to whom the Materials are furnished to do so, subject to
   10  * the following conditions:
   11  *
   12  * The above copyright notice and this permission notice shall be included
   13  * in all copies or substantial portions of the Materials.
   14  *
   15  * MODIFICATIONS TO THIS FILE MAY MEAN IT NO LONGER ACCURATELY REFLECTS
   16  * KHRONOS STANDARDS. THE UNMODIFIED, NORMATIVE VERSIONS OF KHRONOS
   17  * SPECIFICATIONS AND HEADER INFORMATION ARE LOCATED AT
   18  *    https://www.khronos.org/registry/
   19  *
   20  * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   21  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   22  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   23  * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   24  * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   25  * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   26  * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
   27  ************************************************************************/

/*
CL_HPP_CL_1_2_DEFAULT_BUILD - Default to OpenCL C 1.2 compilation rather than
  OpenCL C 2.0 applies to use of cl::Program construction and other program
  build variants.

CL_HPP_ENABLE_DEVICE_FISSION - Enables device fission for OpenCL 1.2 platforms.

CL_HPP_ENABLE_EXCEPTIONS - Enable exceptions for use in the C++ bindings header.
  This is the preferred error handling mechanism but is not required.

CL_HPP_TARGET_OPENCL_VERSION - Defines the target OpenCL runtime version to
  build the header against. Defaults to 200, representing OpenCL 2.0.
*/

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <CL/cl2.hpp>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

const int numElements = 32;

int main(void) {
  // Filter for a 2.0 platform and set it as the default
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  cl::Platform plat;

  for (auto &p : platforms) {
    std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
    if (platver.find("OpenCL 2.") != std::string::npos) {
      plat = p;
    }
  }

  if (plat() == 0) {
    std::cout << "No OpenCL 2.0 platform found.";
    return -1;
  }

  cl::Platform newP = cl::Platform::setDefault(plat);
  if (newP != plat) {
    std::cout << "Error setting default platform.";
    return -1;
  }

  // Use C++11 raw string literals for kernel source code
  std::string kernel1{R"CLC(
        global int globalA;
        kernel void updateGlobal()
        {
          globalA = 75;
        }
       
    )CLC"};
  std::string kernel2{R"CLC(
        typedef struct { global int *bar; } Foo;
        kernel void vectorAdd(global const Foo* aNum, global const int *inputA, global const int *inputB,
                              global int *output, int val, write_only pipe int outPipe, queue_t childQueue)
        {
          output[get_global_id(0)] = inputA[get_global_id(0)] + inputB[get_global_id(0)] + val + *(aNum->bar);
          write_pipe(outPipe, &val);
          queue_t default_queue = get_default_queue();
          ndrange_t ndrange = ndrange_1D(get_global_size(0)/2, get_global_size(0)/2);
          // Have a child kernel write into third quarter of output
          enqueue_kernel(default_queue, CLK_ENQUEUE_FLAGS_WAIT_KERNEL, ndrange,
            ^{
                output[get_global_size(0)*2 + get_global_id(0)] =
                  inputA[get_global_size(0)*2 + get_global_id(0)] + inputB[get_global_size(0)*2 + get_global_id(0)] + globalA;
            });
          // Have a child kernel write into last quarter of output
          enqueue_kernel(childQueue, CLK_ENQUEUE_FLAGS_WAIT_KERNEL, ndrange,
            ^{
                output[get_global_size(0)*3 + get_global_id(0)] =
                  inputA[get_global_size(0)*3 + get_global_id(0)] + inputB[get_global_size(0)*3 + get_global_id(0)] + globalA + 2;
            });
        }
    )CLC"};

  // New simpler string interface style
  std::vector<std::string> programStrings{kernel1, kernel2};
  cl::Program vectorAddProgram(programStrings);
  try {
    vectorAddProgram.build("-cl-std=CL2.0");
  } catch (...) {
    // Print build info for all devices
    cl_int buildErr = CL_SUCCESS;
    auto buildInfo =
        vectorAddProgram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(&buildErr);
    for (auto &pair : buildInfo) {
      std::cerr << pair.second << std::endl << std::endl;
    }
    return 1;
  }

  typedef struct { int *bar; } Foo;
  // Get and run kernel that initializes the program-scope global
  // A test for kernels that take no arguments
  auto program2Kernel = cl::KernelFunctor<>(vectorAddProgram, "updateGlobal");
  program2Kernel(cl::EnqueueArgs(cl::NDRange(1)));

  // SVM allocations
  auto anSVMInt = cl::allocate_svm<int, cl::SVMTraitCoarse<>>();
  *anSVMInt = 5;
  cl::SVMAllocator<Foo, cl::SVMTraitCoarse<cl::SVMTraitReadOnly<>>>
      svmAllocReadOnly;
  auto fooPointer = cl::allocate_pointer<Foo>(svmAllocReadOnly);
  fooPointer->bar = anSVMInt.get();
  cl::SVMAllocator<int, cl::SVMTraitCoarse<>> svmAlloc;
  std::vector<int, cl::SVMAllocator<int, cl::SVMTraitCoarse<>>> inputA(
      numElements, 1, svmAlloc);
  cl::coarse_svm_vector<int> inputB(numElements, 2, svmAlloc);

  // Traditional cl_mem allocations
  std::vector<int> output(numElements, 0xdeadbeef);
  cl::Buffer outputBuffer(begin(output), end(output), false);
  cl::Pipe aPipe(sizeof(cl_int), numElements / 2);

  // Default command queue, also passed in as a parameter
  cl::DeviceCommandQueue defaultDeviceQueue =
      cl::DeviceCommandQueue::makeDefault(cl::Context::getDefault(),
                                          cl::Device::getDefault());
  auto vectorAddKernel =
      cl::KernelFunctor<decltype(fooPointer) &, int *,
                        cl::coarse_svm_vector<int> &, cl::Buffer, int,
                        cl::Pipe &, cl::DeviceCommandQueue>(vectorAddProgram,
                                                            "vectorAdd");

  // Ensure that the additional SVM pointer is available to the kernel
  // This one was not passed as a parameter
  vectorAddKernel.setSVMPointers(anSVMInt);

  // Hand control of coarse allocations to runtime
  cl::enqueueUnmapSVM(anSVMInt);
  cl::enqueueUnmapSVM(fooPointer);
  cl::unmapSVM(inputB);
  cl::unmapSVM(output);
  cl_int error;
  vectorAddKernel(cl::EnqueueArgs(cl::NDRange(numElements / 2),
                                  cl::NDRange(numElements / 2)),
                  fooPointer, inputA.data(), inputB, outputBuffer, 3, aPipe,
                  defaultDeviceQueue, error);
  cl::copy(outputBuffer, begin(output), end(output));

  // Grab the SVM output vector using a map
  cl::mapSVM(output);
  cl::Device d = cl::Device::getDefault();
  std::cout << "Output:\n";
  for (int i = 1; i < numElements; ++i) {
    std::cout << "\t" << output[i] << "\n";
  }
  std::cout << "\n\n";
  return 0;
}
