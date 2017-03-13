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
      : std::runtime_error(what_arg) {}
  explicit compute_error(const char *what_arg) : std::runtime_error(what_arg) {}
};

class invalid_compute_platform : public compute_error {
public:
  explicit invalid_compute_platform(const std::string &what_arg)
      : compute_error(what_arg) {}
  explicit invalid_compute_platform(const char *what_arg)
      : compute_error(what_arg) {}
};

class invalid_compute_device : public compute_error {
public:
  explicit invalid_compute_device(const std::string &what_arg)
      : compute_error(what_arg) {}
  explicit invalid_compute_device(const char *what_arg)
      : compute_error(what_arg) {}
};

} // namespace teuthid

#endif // TEUTHID_COMPUTE_ERROR_HPP
