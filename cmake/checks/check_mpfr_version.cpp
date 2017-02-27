
#include <iostream>
#include <mpfr.h>

using namespace std;

int main()
{
  cout << MPFR_VERSION_MAJOR << endl;
  cout << MPFR_VERSION_MINOR << endl;
  cout << MPFR_VERSION_PATCHLEVEL << endl;
  return 0;
}
