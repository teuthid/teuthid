
  __kernel void opencl_test(__global char* string)
  {
    string[0]  = 'o';
    string[1]  = 'p';
    string[2]  = 'e';
    string[3]  = 'n';
    string[4]  = 'c';
    string[5]  = 'l';
    string[6]  = '_';
    string[7]  = 't';
    string[8]  = 'e';
    string[9]  = 's';
    string[10] = 't';
    string[13] = '\0';
  }
  