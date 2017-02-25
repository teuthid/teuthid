#include <vector>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/core.hpp>

namespace compute = boost::compute;

int main()
{
  int host_data[] = {1, 3, 5, 7, 9};
  std::vector<int> host_vector(5);

  compute::device device = compute::system::default_device();
  compute::context context(device);
  compute::command_queue queue(context, device);
  compute::vector<int> device_vector(5, context);
  compute::copy(host_data, host_data + 5, device_vector.begin(), queue);
  compute::copy(device_vector.begin(), device_vector.end(),
                host_vector.begin(), queue);
  return 0;
}
