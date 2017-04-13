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

#define BOOST_TEST_MODULE teuthid_clb
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <teuthid/clb/platform.hpp>

using namespace teuthid::clb;

BOOST_AUTO_TEST_CASE(class_teuthid_clb_device) {
  const platforms_t &__platforms = platform::platforms();
  BOOST_TEST(!__platforms.empty());

  for (auto __platform : __platforms) {
    auto __devices = __platform.devices();
    for (auto __device : __devices) {
      BOOST_TEST(__device.id(), "id()");
      BOOST_TEST((__device.is_full_profile() || __device.is_embedded_profile()),
                 "profile()");
      BOOST_TEST(!__device.name().empty(), "name()");
      BOOST_TEST(!__device.version().empty(), "version()");
      BOOST_TEST(!__device.driver_version().empty(), "driver_version()");
      BOOST_TEST(!__device.c_version().empty(), "c_version()");
      BOOST_TEST(__device.max_compute_units() > 0, "max_compute_units()");
      BOOST_TEST(!__device.extensions().empty(), "extensions()");
      for (auto __ext : __device.extensions()) {
        BOOST_TEST(!__ext.empty(), "extensions()");
        BOOST_TEST(__device.have_extension(__ext), "have_extension()");
      }
      BOOST_TEST(
          ((__device.address_bits() == 32) || (__device.address_bits() == 64)),
          "address_bits()");
      BOOST_TEST(__device.global_mem_size() > 0, "global_mem_size()");
      BOOST_TEST(__device.global_mem_cache_size() > 0,
                 "global_mem_cache_size()");
      BOOST_TEST(__device.global_mem_cache_line_size() > 0,
                 "global_mem_cache_line_size()");
      BOOST_TEST(__device.info<devparam_t::IMAGE2D_MAX_HEIGHT>() > 0,
                 "info<devparam_t::IMAGE2D_MAX_HEIGHT>()");
      BOOST_TEST(__device.info<devparam_t::IMAGE2D_MAX_WIDTH>() > 0,
                 "info<devparam_t::IMAGE2D_MAX_WIDTH>()");
      BOOST_TEST(__device.info<devparam_t::IMAGE3D_MAX_DEPTH>() > 0,
                 "info<devparam_t::IMAGE2D_MAX_WIDTH>()");
      BOOST_TEST(__device.info<devparam_t::IMAGE3D_MAX_HEIGHT>() > 0,
                 "info<devparam_t::IMAGE3D_MAX_HEIGHT>()");
      BOOST_TEST(__device.info<devparam_t::IMAGE3D_MAX_WIDTH>() > 0,
                 "info<devparam_t::IMAGE3D_MAX_WIDTH>()");
      BOOST_TEST(__device.local_mem_size() > 0, "local_mem_size()");
      BOOST_TEST(__device.max_clock_frequency() > 0, "max_clock_frequency()");
      BOOST_TEST(__device.info<devparam_t::MAX_CONSTANT_ARGS>() > 0,
                 "info<devparam_t::MAX_CONSTANT_ARGS>()");
      BOOST_TEST(__device.info<devparam_t::MAX_CONSTANT_BUFFER_SIZE>() > 0,
                 "info<devparam_t::MAX_CONSTANT_BUFFER_SIZE>()");
      BOOST_TEST(__device.max_mem_alloc_size() > 0, "max_mem_alloc_size()");
      BOOST_TEST(__device.info<devparam_t::MAX_PARAMETER_SIZE>() > 0,
                 "info<devparam_t::MAX_PARAMETER_SIZE>()");
      BOOST_TEST(__device.info<devparam_t::MAX_SAMPLERS>() > 0,
                 "info<devparam_t::MAX_SAMPLERS>()");
      BOOST_TEST(__device.max_work_group_size() > 0, "max_work_group_size()");
      BOOST_TEST(__device.max_work_item_dimensions() > 0,
                 "max_work_item_dimensions()");
      BOOST_TEST(!__device.info<devparam_t::MAX_WORK_ITEM_SIZES>().empty(),
                 "info<devparam_t::MAX_WORK_ITEM_SIZES>()");
      BOOST_TEST(__device.info<devparam_t::MEM_BASE_ADDR_ALIGN>() > 0,
                 "info<devparam_t::MEM_BASE_ADDR_ALIGN>()");

      BOOST_TEST(__device.native_vector_width<bool>() == 0,
                 "native_vector_width<>()");
      BOOST_TEST(__device.native_vector_width<int8_t>() > 0,
                 "native_vector_width<int8_t>()");
      BOOST_TEST(__device.native_vector_width<uint8_t>() > 0,
                 "native_vector_width<uint8_t>()");
      BOOST_TEST(__device.native_vector_width<int16_t>() > 0,
                 "native_vector_width<int16_t>()");
      BOOST_TEST(__device.native_vector_width<uint16_t>() > 0,
                 "native_vector_width<uint16_t>()");
      BOOST_TEST(__device.native_vector_width<int32_t>() > 0,
                 "native_vector_width<int32_t>()");
      BOOST_TEST(__device.native_vector_width<uint32_t>() > 0,
                 "native_vector_width<uint32_t>()");
      BOOST_TEST(__device.native_vector_width<int64_t>() > 0,
                 "native_vector_width<int64_t>()");
      BOOST_TEST(__device.native_vector_width<uint64_t>() > 0,
                 "native_vector_width<uint64_t>()");
      BOOST_TEST(__device.native_vector_width<float32_t>() > 0,
                 "native_vector_width<float32_t>()");

      BOOST_TEST(__device.preferred_vector_width<bool>() == 0,
                 "preferred_vector_width<>()");
      BOOST_TEST(__device.preferred_vector_width<int8_t>() > 0,
                 "preferred_vector_width<int8_t>()");
      BOOST_TEST(__device.preferred_vector_width<uint8_t>() > 0,
                 "preferred_vector_width<int8_t>()");
      BOOST_TEST(__device.preferred_vector_width<int16_t>() > 0,
                 "preferred_vector_width<int16_t>()");
      BOOST_TEST(__device.preferred_vector_width<uint16_t>() > 0,
                 "preferred_vector_width<uint16_t>()");
      BOOST_TEST(__device.preferred_vector_width<int32_t>() > 0,
                 "preferred_vector_width<int32_t>()");
      BOOST_TEST(__device.preferred_vector_width<uint32_t>() > 0,
                 "preferred_vector_width<uint32_t>()");
      BOOST_TEST(__device.preferred_vector_width<int64_t>() > 0,
                 "preferred_vector_width<int64_t>()");
      BOOST_TEST(__device.preferred_vector_width<uint64_t>() > 0,
                 "preferred_vector_width<uint64_t>()");
      BOOST_TEST(__device.preferred_vector_width<float32_t>() > 0,
                 "preferred_vector_width<float32_t>()");

      BOOST_TEST(__device.get_platform().id() == __platform.id(),
                 "get_platform()");
      BOOST_TEST((device::get_platform(__device.id()) == __platform),
                 "device::get_platform()");
      device __dev = __device;
      BOOST_TEST(__dev.id(), "id()");
      BOOST_TEST(__dev.id() == __device.id(), "id()");
      BOOST_TEST((__dev == __device), "operator==");
      BOOST_TEST(!(__dev != __device), "operator!=");
      __dev = device(__device.id());
      BOOST_TEST(__dev.id(), "id()");
      BOOST_TEST(__dev.id() == __device.id(), "id()");
      BOOST_TEST(__dev.get_platform().id() == __platform.id(),
                 "get_platform()");
    }
  }
}