#ifndef PT_BOOST_FORMAT_H
#define PT_BOOST_FORMAT_H

#include "auto_link.h"

#pragma warning(push)
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4267)  // possible loss of data
#pragma warning(disable: 4244)  // possible loss of data
#pragma warning(disable: 4512)  // assignment operator could not be generated
#pragma warning(disable: 4511)  // copy constructor could not be generated
#pragma warning(disable: 4702)  // unreachable code

#include <boost/format.hpp>
//#include <boost/format/free_funcs.hpp>

#pragma warning(pop)

namespace pt {
    //using ::boost::format;
} // namespace pt

#endif
