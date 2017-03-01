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

#ifndef TEUTHID_CL_INFO_HPP

#include <list>  
#include <string>  

namespace teuthid {
namespace cl {

class platform_info {
  public:
    enum profile {FULL, EMBEDDED};
  
  private:
    std::size_t id_;
    profile profile_;
    int major_version_;
    int major_version_;
    int spec_version_;
    std::string version_;
    std::string name_;
    std::string vendor_;
    std::list<std::string> extensions_;
    // uint64_t host_timer_resolution_;
    std::string icd_suffix_khr_;
    
    // some "native" queries
    cl_platform_id cl_id_;
    std::string cl_profile_;
    std::string cl_version_;
    std::string cl_extensions_;
    cl_ulong cl_host_timer_resolution_;   
};  
  
} // namespace cl
} // namespace teuthid

#endif // TEUTHID_CL_INFO_HPP