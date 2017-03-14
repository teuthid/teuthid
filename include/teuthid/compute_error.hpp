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

#ifndef TEUTHID_COMPUTE_ERROR_HPP
#define TEUTHID_COMPUTE_ERROR_HPP

#include <stdexcept>
#include <string>

namespace teuthid {

class compute_error : public std::runtime_error {
public:
  explicit compute_error(const std::string &what_arg)
      : std::runtime_error(what_arg), what_arg_(what_arg), cl_error_(0) {}
  explicit compute_error(const char *what_arg)
      : std::runtime_error(what_arg), what_arg_(what_arg), cl_error_(0) {}
  compute_error(int cl_error) : std::runtime_error("opencl error") {
    cl_error_ = cl_error;
    what_arg_ = "opencl error: ";
    what_arg_ = what_arg_.append(std::to_string(cl_error));
  }
  virtual const char *what() const noexcept { return what_arg_.c_str(); }
  int cl_error() const noexcept { return cl_error_; }
private:
  int cl_error_;
  std::string what_arg_;
};

class invalid_compute_platform : public compute_error {
public:
  explicit invalid_compute_platform(const std::string &what_arg)
      : compute_error(what_arg) {}
  explicit invalid_compute_platform(const char *what_arg)
      : compute_error(what_arg) {}
  invalid_compute_platform(int cl_error) : compute_error(cl_error) {}
};

class invalid_compute_device : public compute_error {
public:
  explicit invalid_compute_device(const std::string &what_arg)
      : compute_error(what_arg) {}
  explicit invalid_compute_device(const char *what_arg)
      : compute_error(what_arg) {}
  invalid_compute_device(int cl_error) : compute_error(cl_error) {}
};

} // namespace teuthid

#endif // TEUTHID_COMPUTE_ERROR_HPP
