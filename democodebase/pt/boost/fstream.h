#ifndef PT_BOOST_FSTREAM_H
#define PT_BOOST_FSTREAM_H

#include "auto_link.h"

#define BOOST_FILESYSTEM_NARROW_ONLY

#pragma warning(push)
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4511) // copy constructor could not be generated
#pragma warning(disable: 4512) // assignment operator could not be generated
#include <boost/filesystem/fstream.hpp>
#pragma warning(pop)

namespace pt { namespace filesystem {
    using namespace ::boost::filesystem;
}} // namespace pt::filesystem

#endif
