#Teuthid#
**Teuthid** is a free open source framework, which implements multilayer artificial neural networks in C++ with support for both fully connected and sparsely connected networks. Cross-platform execution in floating-point numbers are supported with more range and precision than C++'s ordinary built-in types. The framework is designed to support multi-core CPU/GPGPU computing platforms based on **[OpenCL](https://www.khronos.org/opencl/)** and using **[Boost](http://www.boost.org/)** and **[ArrayFire](https://arrayfire.com/)**, free portable C++ source libraries.

**Teuthid** was inspired by the project **[FANN](http://leenissen.dk/fann/wp/)**. Thank you, Mr. Steffen Nissen!
##Features##
* Multilayer artificial neural network libraries in C++11.
* Multiple-precision floating-point computation (requires [MPFR](http://www.mpfr.org/) and [GMP](https://gmplib.org/) libraries).
* Versatile (possible to adjust many parameters and features).
* Use OpenCL as its interface for executing code on parallel devices such as GPUs and multi-core CPUs (with [Boost.Compute](http://www.boost.org/doc/libs/release/libs/compute/) or [ArrayFire](https://github.com/arrayfire/arrayfire) backend).
* Language bindings to C.
* Rigorously tested using [Boost.Test](http://www.boost.org/doc/libs/release/libs/test/) library.  
* Open source, but can be used in commercial applications (licenced under [GNU LGPL v3](https://www.gnu.org/licenses/lgpl-3.0.en.html)).
* ...

Refer to README file for more details.
