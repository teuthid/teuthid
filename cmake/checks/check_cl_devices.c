// Copyright (c) 2016 Wladimir J. van der Laan
// Distributed under the MIT software license.
// Based on an example from the OpenCL cookbook.
/*
Modified for the Teuthid project
Copyright (C) 2016-2017 Mariusz Przygodzki (mariusz.przygodzki@gmail.com)
*/

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

struct platform_data_item {
  int id;
  char *name;
};

struct platform_data_item platform_data_items[] = {
    {CL_PLATFORM_PROFILE, "Profile"},
    {CL_PLATFORM_VERSION, "Version"},
    {CL_PLATFORM_NAME, "Name"},
    {CL_PLATFORM_VENDOR, "Vendor"},
    {CL_PLATFORM_EXTENSIONS, "Extensions"},
};

#define ARRAYLEN(array) (sizeof(array) / sizeof((array)[0]))

int main() {
  int i, j;
  char *value;
  size_t valueSize;
  cl_uint platformCount = 0;
  cl_platform_id *platforms;
  cl_uint deviceCount;
  cl_device_id *devices;
  cl_uint maxComputeUnits;



  return 0;
}
