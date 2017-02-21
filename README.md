#Teuthid#
**Teuthid** is a free open source framework, which implements multilayer artificial neural networks in C++ with support for both fully connected and sparsely connected networks. Cross-platform execution in floating-point numbers are supported with more range and precision than C++'s ordinary built-in types. The framework is designed to support multi-core CPU/GPGPU computing platforms based on **[OpenCL](https://www.khronos.org/opencl/)** and using **[Boost](http://www.boost.org/)**, free portable C++ source libraries.

**Teuthid** was inspired by the project **[FANN](http://leenissen.dk/fann/wp/)**. Thank you, Mr. Steffen Nissen!
##Features##
* Multilayer artificial neural network libraries in C++11
* Versatile (possible to adjust many parameters and features)
* Designed with OpenCL support
* Language bindings to C
* Open source, but can be used in commercial applications (licenced under [LGPLv3](https://www.gnu.org/licenses/lgpl-3.0.en.html))
* ...

##Installation##
###On Linux systems###
####From sources
Clone the repository: `git clone https://github.com/teuthid/teuthid.git`

Once that's finished, navigate to the Root directory and create `build` folder:

`cd teuthid ; mkdir build ; cd build`

Then run **[CMake](https://cmake.org/)**: `cmake ../`

After that, you'll need to use elevated priviledges to install the framework: `sudo make install`

Available **CMake** options for building the framework are: 
* `BUILD_STATIC_LIBS` - build static libraries (default is `OFF`),
* `BUILD_WITH_OPENCL`- build with OpenCL support (default is `OFF`),
* `USE_BOOST_COMPUTE` - use [Boost.Compute](https://github.com/boostorg/compute) for OpenCL support (`BUILD_WITH_OPENCL` must be `ON` in this case),
* `BUILD_TESTS` - build unit tests (default is `ON`),
* `BUILD_DOCS` - build HTML documentation (using **[Doxygen](http://www.stack.nl/~dimitri/doxygen/)**, default is `ON`).


---
*P.S. This is early stage of the project yet, as this README ;)*
