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

long double __value = 0.9876543219;
long double __v2 = 0.12345;

int main() {
  std::cout << "Teuthid Version " << system::to_string(system::major_version())
            << "." << system::to_string(system::minor_version()) << std::endl;
  std::cout << "  --" << std::endl;
  system::format_float_output();

  floatmp<5> __f5 = __value;
  std::cout << "floatmp<5>   x=0.9876543219  to_string(x) => "
            << system::to_string(__f5) << std::endl;
  std::cout << "  numeric_linits<floatmp<5>>::min() => "
            << system::to_string(std::numeric_limits<floatmp<5>>::min());
  std::cout << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f5))
            << "  double(x) => " << system::to_string(double(__f5))
            << "  long double(x) => " << system::to_string((long double)(__f5))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f5))
            << "  float32_t(x) => " << system::to_string(float32_t(__f5))
            << "  float64_t(x) => " << system::to_string(float64_t(__f5))
            << std::endl
            << "  float80_t(x) => " << system::to_string(float80_t(__f5))
            << "  float128_t(x) => " << system::to_string(float128_t(__f5))
            << "  float256_t(x) => " << system::to_string(float256_t(__f5))
            << std::endl;
  __f5 += __v2;
  std::cout << "  x.add(0.12345) => " << system::to_string(__f5);
  __f5 -= __v2;
  std::cout << "  x.sub(0.12345) => " << system::to_string(__f5);
  std::cout << std::endl;
  __f5 *= 100000;
  std::cout << "  x.mul(100000) => " << system::to_string(__f5);
  __f5 /= 100000;
  std::cout << "  x.div(100000) => " << system::to_string(__f5);
  std::cout << std::endl;
  std::cout << "  exp(x) => " << system::to_string(std::exp(__f5));
  std::cout << "  exp2(x) => " << system::to_string(std::exp2(__f5));
  std::cout << "  expm1(x) => " << system::to_string(std::expm1(__f5));
  std::cout << std::endl;
  std::cout << "  log(x) => " << system::to_string(std::log(__f5));
  std::cout << "  log10(x) => " << system::to_string(std::log10(__f5));
  std::cout << "  log2(x) => " << system::to_string(std::log2(__f5));
  std::cout << std::endl;
  std::cout << "  sqrt(x) => " << system::to_string(std::sqrt(__f5));
  std::cout << "  cbrt(x) => " << system::to_string(std::cbrt(__f5));
  std::cout << "  hypot(x, x) => " << system::to_string(std::hypot(__f5, __f5));
  std::cout << std::endl;
  std::cout << "  sin(x) => " << system::to_string(std::sin(__f5));
  std::cout << "  cos(x) => " << system::to_string(std::cos(__f5));
  std::cout << "  tan(x) => " << system::to_string(std::tan(__f5));
  std::cout << std::endl;
  std::cout << "  asin(x) => " << system::to_string(std::asin(__f5));
  std::cout << "  acos(x) => " << system::to_string(std::acos(__f5));
  std::cout << "  atan(x) => " << system::to_string(std::atan(__f5));
  std::cout << std::endl;
  std::cout << "  sinh(x) => " << system::to_string(std::sinh(__f5));
  std::cout << "  cosh(x) => " << system::to_string(std::cosh(__f5));
  std::cout << "  tanh(x) => " << system::to_string(std::tanh(__f5));
  std::cout << std::endl;
  std::cout << "  asinh(x) => " << system::to_string(std::asinh(__f5));
  std::cout << "  acosh(2x) => " << system::to_string(std::acosh(__f5 * 2));
  std::cout << "  atanh(x) => " << system::to_string(std::atanh(__f5));
  std::cout << std::endl;
  std::cout << "  erf(x) => " << system::to_string(std::erf(__f5));
  std::cout << "  erfc(x) => " << system::to_string(std::erfc(__f5));
  std::cout << std::endl;
  std::cout << "  tgamma(x) => " << system::to_string(std::tgamma(__f5));
  std::cout << "  lgamma(x) => " << system::to_string(std::lgamma(__f5));
  std::cout << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<10> __f10 = __value;
  std::cout << "floatmp<10>  x=0.9876543219  to_string(x) => "
            << system::to_string(__f10) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f10))
            << "  double(x) => " << system::to_string(double(__f10))
            << "  long double(x) => " << system::to_string((long double)(__f10))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f10))
            << "  float32_t(x) => " << system::to_string(float32_t(__f10))
            << "  float64_t(x) => " << system::to_string(float64_t(__f10))
            << std::endl
            << "  float80_t(x) => " << system::to_string(float80_t(__f10))
            << "  float128_t(x) => " << system::to_string(float128_t(__f10))
            << "  float256_t(x) => " << system::to_string(float256_t(__f10))
            << std::endl;
  __f10 += __v2;
  std::cout << "  x.add(0.12345) => " << system::to_string(__f10);
  __f10 -= __v2;
  std::cout << "  x.sub(0.12345) => " << system::to_string(__f10);
  std::cout << std::endl;
  __f10 *= 100000;
  std::cout << "  x.mul(100000) => " << system::to_string(__f10);
  __f10 /= 100000;
  std::cout << "  x.div(100000) => " << system::to_string(__f10);
  std::cout << std::endl;
  std::cout << "  exp(x) => " << system::to_string(std::exp(__f10));
  std::cout << "  exp2(x) => " << system::to_string(std::exp2(__f10));
  std::cout << "  expm1(x) => " << system::to_string(std::expm1(__f10));
  std::cout << std::endl;
  std::cout << "  log(x) => " << system::to_string(std::log(__f10));
  std::cout << "  log10(x) => " << system::to_string(std::log10(__f10));
  std::cout << "  log2(x) => " << system::to_string(std::log2(__f10));
  std::cout << std::endl;
  std::cout << "  sqrt(x) => " << system::to_string(std::sqrt(__f10));
  std::cout << "  cbrt(x) => " << system::to_string(std::cbrt(__f10));
  std::cout << "  hypot(x, x) => "
            << system::to_string(std::hypot(__f10, __f10));
  std::cout << std::endl;
  std::cout << "  sin(x) => " << system::to_string(std::sin(__f10));
  std::cout << "  cos(x) => " << system::to_string(std::cos(__f10));
  std::cout << "  tan(x) => " << system::to_string(std::tan(__f10));
  std::cout << std::endl;
  std::cout << "  asin(x) => " << system::to_string(std::asin(__f10));
  std::cout << "  acos(x) => " << system::to_string(std::acos(__f10));
  std::cout << "  atan(x) => " << system::to_string(std::atan(__f10));
  std::cout << std::endl;
  std::cout << "  sinh(x) => " << system::to_string(std::sinh(__f10));
  std::cout << "  cosh(x) => " << system::to_string(std::cosh(__f10));
  std::cout << "  tanh(x) => " << system::to_string(std::tanh(__f10));
  std::cout << std::endl;
  std::cout << "  asinh(x) => " << system::to_string(std::asinh(__f10));
  std::cout << "  acosh(2x) => " << system::to_string(std::acosh(__f10 * 2));
  std::cout << "  atanh(x) => " << system::to_string(std::atanh(__f10));
  std::cout << std::endl;
  std::cout << "  erf(x) => " << system::to_string(std::erf(__f10));
  std::cout << "  erfc(x) => " << system::to_string(std::erfc(__f10));
  std::cout << std::endl;
  std::cout << "  tgamma(x) => " << system::to_string(std::tgamma(__f10));
  std::cout << "  lgamma(x) => " << system::to_string(std::lgamma(__f10));
  std::cout << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<20> __f20 = __value;
  std::cout << "floatmp<20>  x=0.9876543219  to_string(x) => "
            << system::to_string(__f20) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f20))
            << "  double(x) => " << system::to_string(double(__f20))
            << "  long double(x) => " << system::to_string((long double)(__f20))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f20))
            << "  float32_t(x) => " << system::to_string(float32_t(__f20))
            << "  float64_t(x) => " << system::to_string(float64_t(__f20))
            << std::endl
            << "  float80_t(x) => " << system::to_string(float80_t(__f20))
            << "  float128_t(x) => " << system::to_string(float128_t(__f20))
            << "  float256_t(x) => " << system::to_string(float256_t(__f20))
            << std::endl;
  __f20 += __v2;
  std::cout << "  x.add(0.12345) => " << system::to_string(__f20);
  __f20 -= __v2;
  std::cout << "  x.sub(0.12345) => " << system::to_string(__f20);
  std::cout << std::endl;
  __f20 *= 100000;
  std::cout << "  x.mul(100000) => " << system::to_string(__f20);
  __f20 /= 100000;
  std::cout << "  x.div(100000) => " << system::to_string(__f20);
  std::cout << std::endl;
  std::cout << "  exp(x) => " << system::to_string(std::exp(__f20));
  std::cout << "  exp2(x) => " << system::to_string(std::exp2(__f20));
  std::cout << "  expm1(x) => " << system::to_string(std::expm1(__f20));
  std::cout << std::endl;
  std::cout << "  log(x) => " << system::to_string(std::log(__f20));
  std::cout << "  log10(x) => " << system::to_string(std::log10(__f20));
  std::cout << "  log2(x) => " << system::to_string(std::log2(__f20));
  std::cout << std::endl;
  std::cout << "  sqrt(x) => " << system::to_string(std::sqrt(__f20));
  std::cout << "  cbrt(x) => " << system::to_string(std::cbrt(__f20));
  std::cout << "  hypot(x, x) => "
            << system::to_string(std::hypot(__f20, __f20));
  std::cout << std::endl;
  std::cout << "  sin(x) => " << system::to_string(std::sin(__f20));
  std::cout << "  cos(x) => " << system::to_string(std::cos(__f20));
  std::cout << "  tan(x) => " << system::to_string(std::tan(__f20));
  std::cout << std::endl;
  std::cout << "  asin(x) => " << system::to_string(std::asin(__f20));
  std::cout << "  acos(x) => " << system::to_string(std::acos(__f20));
  std::cout << "  atan(x) => " << system::to_string(std::atan(__f20));
  std::cout << std::endl;
  std::cout << "  sinh(x) => " << system::to_string(std::sinh(__f20));
  std::cout << "  cosh(x) => " << system::to_string(std::cosh(__f20));
  std::cout << "  tanh(x) => " << system::to_string(std::tanh(__f20));
  std::cout << std::endl;
  std::cout << "  asinh(x) => " << system::to_string(std::asinh(__f20));
  std::cout << "  acosh(2x) => " << system::to_string(std::acosh(__f20 * 2));
  std::cout << "  atanh(x) => " << system::to_string(std::atanh(__f20));
  std::cout << std::endl;
  std::cout << "  erf(x) => " << system::to_string(std::erf(__f20));
  std::cout << "  erfc(x) => " << system::to_string(std::erfc(__f20));
  std::cout << std::endl;
  std::cout << "  tgamma(x) => " << system::to_string(std::tgamma(__f20));
  std::cout << "  lgamma(x) => " << system::to_string(std::lgamma(__f20));
  std::cout << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<50> __f50 = __value;
  std::cout << "floatmp<50>  x=0.9876543219  to_string(x) => "
            << system::to_string(__f50) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f50))
            << "  double(x) => " << system::to_string(double(__f50))
            << "  long double(x) => " << system::to_string((long double)(__f50))
            << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f50))
            << "  float32_t(x) => " << system::to_string(float32_t(__f50))
            << "  float64_t(x) => " << system::to_string(float64_t(__f50))
            << std::endl
            << "  float80_t(x) => " << system::to_string(float80_t(__f50))
            << "  float128_t(x) => " << system::to_string(float128_t(__f50))
            << "  float256_t(x) => " << system::to_string(float256_t(__f50))
            << std::endl;
  __f50 += __v2;
  std::cout << "  x.add(0.12345) => " << system::to_string(__f50);
  __f50 -= __v2;
  std::cout << "  x.sub(0.12345) => " << system::to_string(__f50);
  std::cout << std::endl;
  __f50 *= 100000;
  std::cout << "  x.mul(100000) => " << system::to_string(__f50);
  __f50 /= 100000;
  std::cout << "  x.div(100000) => " << system::to_string(__f50);
  std::cout << std::endl;
  std::cout << "  exp(x) => " << system::to_string(std::exp(__f50));
  std::cout << "  exp2(x) => " << system::to_string(std::exp2(__f50));
  std::cout << "  expm1(x) => " << system::to_string(std::expm1(__f50));
  std::cout << std::endl;
  std::cout << "  log(x) => " << system::to_string(std::log(__f50));
  std::cout << "  log10(x) => " << system::to_string(std::log10(__f50));
  std::cout << "  log2(x) => " << system::to_string(std::log2(__f50));
  std::cout << std::endl;
  std::cout << "  sqrt(x) => " << system::to_string(std::sqrt(__f50));
  std::cout << "  cbrt(x) => " << system::to_string(std::cbrt(__f50));
  std::cout << "  hypot(x, x) => "
            << system::to_string(std::hypot(__f50, __f50));
  std::cout << std::endl;
  std::cout << "  sin(x) => " << system::to_string(std::sin(__f50));
  std::cout << "  cos(x) => " << system::to_string(std::cos(__f50));
  std::cout << "  tan(x) => " << system::to_string(std::tan(__f50));
  std::cout << std::endl;
  std::cout << "  asin(x) => " << system::to_string(std::asin(__f50));
  std::cout << "  acos(x) => " << system::to_string(std::acos(__f50));
  std::cout << "  atan(x) => " << system::to_string(std::atan(__f50));
  std::cout << std::endl;
  std::cout << "  sinh(x) => " << system::to_string(std::sinh(__f50));
  std::cout << "  cosh(x) => " << system::to_string(std::cosh(__f50));
  std::cout << "  tanh(x) => " << system::to_string(std::tanh(__f50));
  std::cout << std::endl;
  std::cout << "  asinh(x) => " << system::to_string(std::asinh(__f50));
  std::cout << "  acosh(2x) => " << system::to_string(std::acosh(__f50 * 2));
  std::cout << "  atanh(x) => " << system::to_string(std::atanh(__f50));
  std::cout << std::endl;
  std::cout << "  erf(x) => " << system::to_string(std::erf(__f50));
  std::cout << "  erfc(x) => " << system::to_string(std::erfc(__f50));
  std::cout << std::endl;
  std::cout << "  tgamma(x) => " << system::to_string(std::tgamma(__f50));
  std::cout << "  lgamma(x) => " << system::to_string(std::lgamma(__f50));
  std::cout << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<100> __f100 = __value;
  std::cout << "floatmp<100> x=0.9876543219  to_string(x) => "
            << system::to_string(__f100) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f100))
            << "  double(x) => " << system::to_string(double(__f100))
            << "  long double(x) => "
            << system::to_string((long double)(__f100)) << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f100))
            << "  float32_t(x) => " << system::to_string(float32_t(__f100))
            << "  float64_t(x) => " << system::to_string(float64_t(__f100))
            << std::endl
            << "  float80_t(x) => " << system::to_string(float80_t(__f100))
            << "  float128_t(x) => " << system::to_string(float128_t(__f100))
            << "  float256_t(x) => " << system::to_string(float256_t(__f100))
            << std::endl;
  __f100 += __v2;
  std::cout << "  x.add(0.12345) => " << system::to_string(__f100);
  __f100 -= __v2;
  std::cout << "  x.sub(0.12345) => " << system::to_string(__f100);
  std::cout << std::endl;
  __f100 *= 100000;
  std::cout << "  x.mul(100000) => " << system::to_string(__f100);
  __f100 /= 100000;
  std::cout << "  x.div(100000) => " << system::to_string(__f100);
  std::cout << std::endl;
  std::cout << "  exp(x) => " << system::to_string(std::exp(__f100));
  std::cout << "  exp2(x) => " << system::to_string(std::exp2(__f100));
  std::cout << "  expm1(x) => " << system::to_string(std::expm1(__f100));
  std::cout << std::endl;
  std::cout << "  log(x) => " << system::to_string(std::log(__f100));
  std::cout << "  log10(x) => " << system::to_string(std::log10(__f100));
  std::cout << "  log2(x) => " << system::to_string(std::log2(__f100));
  std::cout << std::endl;
  std::cout << "  sqrt(x) => " << system::to_string(std::sqrt(__f100));
  std::cout << "  cbrt(x) => " << system::to_string(std::cbrt(__f100));
  std::cout << "  hypot(x, x) => "
            << system::to_string(std::hypot(__f100, __f100));
  std::cout << std::endl;
  std::cout << "  sin(x) => " << system::to_string(std::sin(__f100));
  std::cout << "  cos(x) => " << system::to_string(std::cos(__f100));
  std::cout << "  tan(x) => " << system::to_string(std::tan(__f100));
  std::cout << std::endl;
  std::cout << "  asin(x) => " << system::to_string(std::asin(__f100));
  std::cout << "  acos(x) => " << system::to_string(std::acos(__f100));
  std::cout << "  atan(x) => " << system::to_string(std::atan(__f100));
  std::cout << std::endl;
  std::cout << "  sinh(x) => " << system::to_string(std::sinh(__f100));
  std::cout << "  cosh(x) => " << system::to_string(std::cosh(__f100));
  std::cout << "  tanh(x) => " << system::to_string(std::tanh(__f100));
  std::cout << std::endl;
  std::cout << "  asinh(x) => " << system::to_string(std::asinh(__f100));
  std::cout << "  acosh(2x) => " << system::to_string(std::acosh(__f100 * 2));
  std::cout << "  atanh(x) => " << system::to_string(std::atanh(__f100));
  std::cout << std::endl;
  std::cout << "  erf(x) => " << system::to_string(std::erf(__f100));
  std::cout << "  erfc(x) => " << system::to_string(std::erfc(__f100));
  std::cout << std::endl;
  std::cout << "  tgamma(x) => " << system::to_string(std::tgamma(__f100));
  std::cout << "  lgamma(x) => " << system::to_string(std::lgamma(__f100));
  std::cout << std::endl;
  std::cout << "  --" << std::endl;

  floatmp<200> __f200 = __value;
  std::cout << "floatmp<200> x=0.9876543219  to_string(x) => "
            << system::to_string(__f200) << std::endl;
  std::cout << "  float(x) => " << system::to_string(float(__f200))
            << "  double(x) => " << system::to_string(double(__f200))
            << "  long double(x) => "
            << system::to_string((long double)(__f200)) << std::endl
            << "  float16_t(x) => " << system::to_string(float16_t(__f200))
            << "  float32_t(x) => " << system::to_string(float32_t(__f200))
            << "  float64_t(x) => " << system::to_string(float64_t(__f200))
            << std::endl
            << "  float80_t(x) => " << system::to_string(float80_t(__f200))
            << "  float128_t(x) => " << system::to_string(float128_t(__f200))
            << "  float256_t(x) => " << system::to_string(float256_t(__f200))
            << std::endl;
  __f200 += __v2;
  std::cout << "  x.add(0.12345) => " << system::to_string(__f200);
  __f200 -= __v2;
  std::cout << "  x.sub(0.12345) => " << system::to_string(__f200);
  std::cout << std::endl;
  __f200 *= 100000;
  std::cout << "  x.mul(100000) => " << system::to_string(__f200);
  __f200 /= 100000;
  std::cout << "  x.div(100000) => " << system::to_string(__f200);
  std::cout << std::endl;
  std::cout << "  exp(x) => " << system::to_string(std::exp(__f200));
  std::cout << "  exp2(x) => " << system::to_string(std::exp2(__f200));
  std::cout << "  expm1(x) => " << system::to_string(std::expm1(__f200));
  std::cout << std::endl;
  std::cout << "  log(x) => " << system::to_string(std::log(__f200));
  std::cout << "  log10(x) => " << system::to_string(std::log10(__f200));
  std::cout << "  log2(x) => " << system::to_string(std::log2(__f200));
  std::cout << std::endl;
  std::cout << "  sqrt(x) => " << system::to_string(std::sqrt(__f200));
  std::cout << "  cbrt(x) => " << system::to_string(std::cbrt(__f200));
  std::cout << "  hypot(x, x) => "
            << system::to_string(std::hypot(__f200, __f200));
  std::cout << std::endl;
  std::cout << "  sin(x) => " << system::to_string(std::sin(__f200));
  std::cout << "  cos(x) => " << system::to_string(std::cos(__f200));
  std::cout << "  tan(x) => " << system::to_string(std::tan(__f200));
  std::cout << std::endl;
  std::cout << "  asin(x) => " << system::to_string(std::asin(__f200));
  std::cout << "  acos(x) => " << system::to_string(std::acos(__f200));
  std::cout << "  atan(x) => " << system::to_string(std::atan(__f200));
  std::cout << std::endl;
  std::cout << "  sinh(x) => " << system::to_string(std::sinh(__f200));
  std::cout << "  cosh(x) => " << system::to_string(std::cosh(__f200));
  std::cout << "  tanh(x) => " << system::to_string(std::tanh(__f200));
  std::cout << std::endl;
  std::cout << "  asinh(x) => " << system::to_string(std::asinh(__f200));
  std::cout << "  acosh(2x) => " << system::to_string(std::acosh(__f200 * 2));
  std::cout << "  atanh(x) => " << system::to_string(std::atanh(__f200));
  std::cout << std::endl;
  std::cout << "  erf(x) => " << system::to_string(std::erf(__f200));
  std::cout << "  erfc(x) => " << system::to_string(std::erfc(__f200));
  std::cout << std::endl;
  std::cout << "  tgamma(x) => " << system::to_string(std::tgamma(__f200));
  std::cout << "  lgamma(x) => " << system::to_string(std::lgamma(__f200));
  std::cout << std::endl;
  std::cout << "  --" << std::endl;

  std::cout << std::endl;
  return 0;
}