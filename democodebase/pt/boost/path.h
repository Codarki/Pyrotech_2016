#ifndef PT_BOOST_FILESYSTEM_PATH_H
#define PT_BOOST_FILESYSTEM_PATH_H

//#include "../std/vector.h"

#include "auto_link.h"

#define BOOST_FILESYSTEM_NARROW_ONLY
#define BOOST_FILESYSTEM_NO_DEPRECATED

#pragma warning(push)
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4245) // conversion from 'x' to 'y', signed/unsigned mismatch
#include <boost/filesystem/path.hpp>
#pragma warning(pop)

namespace pt {
    //using boost::filesystem::path;
} // namespace pt

namespace pt { namespace filesystem {
}} // namespace pt::filesystem

#endif
