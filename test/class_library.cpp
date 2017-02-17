
#define BOOST_TEST_MODULE teuthid
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "library.hpp"

using namespace teuthid;

BOOST_AUTO_TEST_CASE(class_teuthid_library)
{
  BOOST_TEST(library::major_version() == TEUTHID_MAJOR_VERSION);
  BOOST_TEST(library::minor_version() == TEUTHID_MINOR_VERSION);
  BOOST_TEST(library::patch_version() == TEUTHID_PATCH_VERSION);
  BOOST_TEST(library::soversion() == TEUTHID_SOVERSION);
  BOOST_TEST(library::version() == std::string(TEUTHID_VERSION));
  BOOST_TEST(library::required_version(TEUTHID_MAJOR_VERSION, 
    TEUTHID_MINOR_VERSION));
  BOOST_TEST(!library::required_version(TEUTHID_MAJOR_VERSION + 1, 
    TEUTHID_MINOR_VERSION));
  BOOST_TEST(!library::required_version(TEUTHID_MAJOR_VERSION, 
    TEUTHID_MINOR_VERSION + 1));
  BOOST_TEST(library::required_version(TEUTHID_MAJOR_VERSION - 1, 
    TEUTHID_MINOR_VERSION));
  BOOST_TEST(library::required_version(TEUTHID_MAJOR_VERSION, 
    TEUTHID_MINOR_VERSION - 1));
}

