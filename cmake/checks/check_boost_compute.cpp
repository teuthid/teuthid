#include <vector>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/core.hpp>

namespace compute = boost::compute;

int main()
{
  compute::device device = compute::system::default_device();
  compute::context context(device);
  compute::command_queue queue(context, device);
  return 0;
}
