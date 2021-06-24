#ifndef PT_BOOST_FILESYSTEM_PATH_FWD_H
#define PT_BOOST_FILESYSTEM_PATH_FWD_H

#include "../std/string.h"

#include "auto_link.h"

#include <boost/filesystem/config.hpp>

namespace boost
{
  namespace BOOST_FILESYSTEM_NAMESPACE
  {
    template<class String, class Traits> class basic_path;

    struct path_traits;
    typedef basic_path< std::string, path_traits > path;
  }
}

namespace pt {
    using boost::filesystem::path;
} // namespace pt

#endif
