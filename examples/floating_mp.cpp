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

long double __value = 0.9999999999;

int main() {
  std::cout << "Teuthid Version " << system::to_string(system::major_version())
            << "." << system::to_string(system::minor_version()) << std::endl;
  std::cout << "  --" << std::endl;
  system::format_float_output();

  floatmp<5> __f5 = __value;
  std::cout << "floatmp<5>   x=0.9999999999  to_string(x) => "
            << system::to_string(__f5) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f5))
            << "  double(x) => " << system::to_string(double(__f5))
            << "  long double(x) => " << system::to_string((long double)(__f5))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f5))
            << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<10> __f10 = __value;
  std::cout << "floatmp<10>  x=0.9999999999  to_string(x) => "
            << system::to_string(__f10) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f10))
            << "  double(x) => " << system::to_string(double(__f10))
            << "  long double(x) => " << system::to_string((long double)(__f10))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f10))
            << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<20> __f20 = __value;
  std::cout << "floatmp<20>  x=0.9999999999  to_string(x) => "
            << system::to_string(__f20) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f20))
            << "  double(x) => " << system::to_string(double(__f20))
            << "  long double(x) => " << system::to_string((long double)(__f20))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f20))
            << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<50> __f50 = __value;
  std::cout << "floatmp<50>  x=0.9999999999  to_string(x) => "
            << system::to_string(__f50) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f50))
            << "  double(x) => " << system::to_string(double(__f50))
            << "  long double(x) => " << system::to_string((long double)(__f50))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f50))
            << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<100> __f100 = __value;
  std::cout << "floatmp<100> x=0.9999999999  to_string(x) => "
            << system::to_string(__f100) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f100))
            << "  double(x) => " << system::to_string(double(__f100))
            << "  long double(x) => "
            << system::to_string((long double)(__f100)) << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f100))
            << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<200> __f200 = __value;
  std::cout << "floatmp<200> x=0.9999999999  to_string(x) => "
            << system::to_string(__f200) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f200))
            << "  double(x) => " << system::to_string(double(__f200))
            << "  long double(x) => "
            << system::to_string((long double)(__f200)) << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f200))
            << std::endl;
  std::cout << "  --" << std::endl;

  std::cout << std::endl;
  return 0;
}