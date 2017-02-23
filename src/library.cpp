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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <teuthid/library.hpp>

using namespace teuthid;

const std::string &library::version_ = TEUTHID_VERSION;

bool library::required_version(int min_major, int min_minor)
{
  if (min_major > library::major_)
    return false;
  else if ((min_major == library::major_) && (min_minor > library::minor_))
    return false;
  return true;
}
