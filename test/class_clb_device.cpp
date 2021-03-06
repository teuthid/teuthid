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

#define BOOST_TEST_MODULE teuthid_clb
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <teuthid/clb/platform.hpp>

using namespace teuthid::clb;

BOOST_AUTO_TEST_CASE(class_teuthid_clb_device) {
  const platforms_t &__platforms = platform::get_all();
  BOOST_TEST(!__platforms.empty());
  BOOST_TEST(
      (device::set_default(device::get_default()) == device::get_default()),
      "device::get_default()");
  for (platform __platform : __platforms) {
    for (device __device : __platform.devices()) {
      BOOST_TEST(!__device.is_subdevice(), "is_subdevice()");
      BOOST_TEST((__device.is_full_profile() || __device.is_embedded_profile()),
                 "profile()");
      BOOST_TEST(__device.id(), "id()");
      BOOST_TEST(!__device.name().empty(), "name()");
      BOOST_TEST(!__device.version().empty(), "version()");
      BOOST_TEST(!__device.driver_version().empty(), "driver_version()");
      BOOST_TEST(!__device.c_version().empty(), "c_version()");
      BOOST_TEST(__device.check_version(0, 0));
      BOOST_TEST(!__device.check_version(3, 0));
      BOOST_TEST(!__device.check_version(2, 999));
      BOOST_TEST(__device.max_compute_units() > 0, "max_compute_units()");
      BOOST_TEST(!__device.vendor().empty(), "vendor()");
      BOOST_TEST(__device.info<devparam_t::VENDOR_ID>() > 0,
                 "info<devparam_t::VENDOR_ID>()");
      for (auto __ext : __device.extensions()) {
        BOOST_TEST(!__ext.empty(), "extensions()");
        BOOST_TEST(__device.has_extension(__ext), "have_extension()");
      }
      BOOST_TEST(
          ((__device.address_bits() == 32) || (__device.address_bits() == 64)),
          "address_bits()");
      BOOST_TEST(__device.global_mem_size() > 0, "global_mem_size()");
      BOOST_TEST(__device.global_mem_cache_size() > 0,
                 "global_mem_cache_size()");
      BOOST_TEST(__device.global_mem_cacheline_size() > 0,
                 "global_mem_cacheline_size()");
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
      BOOST_TEST(__device.max_constant_args() > 0, "max_constant_args()");
      BOOST_TEST(__device.max_constant_buffer_size() > 0,
                 "max_constant_buffer_size()");
      BOOST_TEST(__device.max_mem_alloc_size() > 0, "max_mem_alloc_size()");
      BOOST_TEST(__device.max_on_device_events() > 0, "max_on_device_events()");
      BOOST_TEST(__device.max_on_device_queues() > 0, "max_on_device_queues()");
      BOOST_TEST(__device.max_parameter_size() > 0, "max_parameter_size()");
      BOOST_TEST(__device.max_pipe_args() > 0, "max_pipe_args()");
      BOOST_TEST(__device.info<devparam_t::MAX_SAMPLERS>() > 0,
                 "info<devparam_t::MAX_SAMPLERS>()");
      BOOST_TEST(__device.max_work_group_size() > 0, "max_work_group_size()");
      BOOST_TEST(__device.max_work_item_dimensions() > 0,
                 "max_work_item_dimensions()");
      BOOST_TEST(!__device.max_work_item_sizes().empty(),
                 "max_work_item_sizes()");
      BOOST_TEST(__device.mem_base_addr_align() > 0, "mem_base_addr_align()");

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

      BOOST_TEST(__device.profiling_timer_resolution() > 0,
                 "profiling_timer_resolution()");
      BOOST_TEST(__device.is_devtype(__device.devtype()), "devtype()");
      BOOST_TEST(__device.is_devtype(devtype_t::ALL), "is_devtype()");
      BOOST_TEST(__device.has_single_precision(), "has_single_precision()");

      BOOST_TEST(__device.get_platform().id() == __platform.id(),
                 "get_platform()");
      device __dev = __device;
      BOOST_TEST(__dev.id(), "id()");
      BOOST_TEST(__dev.id() == __device.id(), "id()");
      BOOST_TEST((__dev == __device), "operator==");
      BOOST_TEST(!(__dev != __device), "operator!=");
      BOOST_TEST(device::find_by_id(__dev.id()).id() == __device.id(),
                 "device::find_by_id()");
      BOOST_TEST(__dev.get_platform().id() == __platform.id(),
                 "get_platform()");

      BOOST_TEST(__device.max_compute_units() >= __device.max_subdevices(),
                 "max_compute_units()");

      if (__dev.max_subdevices() > 1) {
        devices_t __subdevices = __dev.subdevices(1);
        BOOST_TEST(__subdevices.size() == __dev.max_subdevices(),
                   "max_subdevices");
        for (const device &__subdev : __subdevices) {
          BOOST_TEST(__subdev.id(), "id()");
          BOOST_TEST(__subdev.id() != __dev.id(), "id()");
          BOOST_TEST(__subdev.parent_id() == __dev.id(), "parent_id()");
          BOOST_TEST(__subdev.is_subdevice(), "is_subdevice()");
        }
      }

      if (__dev.max_subdevices() > 1) {
        std::vector<std::size_t> __units;
        for (auto __i = 0; __i < __dev.max_subdevices(); __i++)
          __units.push_back(1);
        devices_t __subdevices = __dev.subdevices(__units);
        BOOST_TEST(__subdevices.size() == __dev.max_subdevices(),
                   "max_subdevices");
        for (const device &__subdev : __subdevices) {
          BOOST_TEST(__subdev.id(), "id()");
          BOOST_TEST(__subdev.id() != __dev.id(), "id()");
          BOOST_TEST(__subdev.parent_id() == __dev.id(), "parent_id()");
          BOOST_TEST(__subdev.is_subdevice(), "is_subdevice()");
        }
      }
    }
  }
}
