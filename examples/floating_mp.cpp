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

#include <iostream>

#include <teuthid/system.hpp>
using namespace teuthid;

std::string yesno(bool value) {
  return (value ? std::string("YES") : std::string("NO"));
}

long double __value = 0.123456789;

int main() {
  std::cout << "Teuthid Version " << system::to_string(system::major_version())
            << "." << system::to_string(system::minor_version()) << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<100> __f100 = static_cast<int128_t>(INT64_MAX) + 1;
  //__f100 = INT64_MAX;
  //__f100 = static_cast<int128_t>(INT64_MAX) + 1;
  std::cout << system::to_string(__f100) << std::endl;
  //
  std::cout << std::endl;
  return 0;
}