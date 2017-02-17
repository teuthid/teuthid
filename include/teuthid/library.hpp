


#ifndef TEUTHID_LIBRARY_HPP

#include <string>
#include "config.hpp"

namespace teuthid
{

class library
{
public:
  library(library const &) = delete;
  void operator=(library const &) = delete;
  static constexpr int major_version()
  {
    return major_;
  }
  static constexpr int minor_version()
  {
    return minor_;
  }
  static constexpr int patch_version()
  {
    return patch_;
  }
  static constexpr int soversion()
  {
    return soversion_;
  }
  static const std::string &version()
  {
    return version_;
  }
  static bool required_version(int min_major, int min_minor);

private:
  library() {}
  ~library() {}

private:
  static constexpr int major_ = TEUTHID_MAJOR_VERSION;
  static constexpr int minor_ = TEUTHID_MINOR_VERSION;
  static constexpr int patch_ = TEUTHID_PATCH_VERSION;
  static constexpr int soversion_ = TEUTHID_SOVERSION;
  static const std::string &version_;
};

} // namespace teuthid

#endif // TEUTHID_LIBRARY_HPP
