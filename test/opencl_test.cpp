#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
    
#include <fstream>
#include <iostream>
#include <string>
    
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)
     
int main()
{
  cl_device_id      device_id = NULL;
  cl_context        context = NULL;
  cl_command_queue  command_queue = NULL;
  cl_mem            memobj = NULL;
  cl_program        program = NULL;
  cl_kernel         kernel = NULL;
  cl_platform_id    platform_id = NULL;
  cl_uint ret_      num_devices;
  cl_uint           ret_num_platforms;
  cl_int            ret;
  
  //char string[MEM_SIZE];
  std::ifstream file_stream;
  std::string file_name = "./test.cl";
  std::streambuf source_str;
  std::streamsize source_size;
  
  // Load the source code containing the kernel
  file_stream.open(file_name.c_str())
  if (!file_stream.good())
  {
    std::cout << "Failed to load kernel from file: " << file_name << std::endl;
    exit(EXIT_FAILURE);
  }
  source_size = file_stream.get(source_str).gcount();
  file_stream.close();
  
  // `ret` is equal to CL_SUCCESS if the function is executed successfully.
  
  // Get Platform and Device Info
  ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id,
                       &ret_num_devices);
    
  // Create OpenCL context
  context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
  
  // Create Command Queue
  command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  
  // Create Memory Buffer
  size_t buffer_size = static_cast<size_t>(source_size) * sizeof(char);
  memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, &ret);
    
   // TO FINISH
    
  /*  
      
    Create Kernel Program from the source
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
    (const size_t *)&source_size, &ret);
     
    Build Kernel Program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
     
    Create OpenCL Kernel
    kernel = clCreateKernel(program, "hello", &ret);
     
    Set OpenCL Kernel Parameters
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
     
    Execute OpenCL Kernel
    ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
     
    Copy results from the memory buffer
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
    MEM_SIZE * sizeof(char),string, 0, NULL, NULL);
     
    Display Result
    puts(string);
     
    Finalization
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
     
    free(source_str);
  */   
  
    return 0;
    }
    