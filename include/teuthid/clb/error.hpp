/*
  This file is part of the Teuthid project.
  Copyright (c) 2016-2017 Mariusz Przygodzki (mariusz.przygodzki@gmail.com)

  The Teuthid is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  The Teuthid is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
  for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the Teuthid; see the file LICENSE.LGPLv3.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#ifndef TEUTHID_CLB_ERROR_HPP
#define TEUTHID_CLB_ERROR_HPP

#include <stdexcept>
#include <string>

namespace teuthid {
namespace clb {

class error : public std::runtime_error {
public:
  explicit error(const std::string &what_arg)
      : std::runtime_error(what_arg), cl_error_(0) {}
  explicit error(const char *what_arg)
      : std::runtime_error(what_arg), cl_error_(0) {}
  explicit error(int cl_error)
      : std::runtime_error(code_to_string_(cl_error)), cl_error_(cl_error) {}

  virtual int cl_error() const noexcept { return cl_error_; }

private:
  int cl_error_;
  static std::string code_to_string_(int cl_error);
};

class invalid_platform : public error {
public:
  explicit invalid_platform(const std::string &what_arg) : error(what_arg) {}
  explicit invalid_platform(const char *what_arg) : error(what_arg) {}
  explicit invalid_platform(int cl_error) : error(cl_error) {}
};

class invalid_device : public error {
public:
  explicit invalid_device(const std::string &what_arg) : error(what_arg) {}
  explicit invalid_device(const char *what_arg) : error(what_arg) {}
  explicit invalid_device(int cl_error) : error(cl_error) {}
};

} // namespace clb
} // namespace teuthid

#endif // TEUTHID_CLB_ERROR_HPP
