
#include "library.hpp"

using namespace teuthid;

const std::string &library::version_ = TEUTHID_VERSION;

bool library::required_version(int min_major, int min_minor) {
  if (min_major > library::major_)
    return false;
  else if ((min_major == library::major_) && (min_minor > library::minor_))
    return false;
  return true;
}
