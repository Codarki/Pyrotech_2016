#ifndef PT_BOOST_FORMAT_FREE_FUNCS_H
#define PT_BOOST_FORMAT_FREE_FUNCS_H

// disable autolinking of all boost libraries
#define BOOST_ALL_NO_LIB

#pragma warning(push)
#pragma warning(disable: 4267)  // 'initializing' : conversion from 'x' to 'y', possible loss of data
#pragma warning(disable: 4244)  // 'initializing' : conversion from 'x' to 'y', possible loss of data
#pragma warning(disable: 4512)  // assignment operator could not be generated
#pragma warning(disable: 4511)  // copy constructor could not be generated

#include <boost/format/free_funcs.hpp>

#pragma warning(pop)

namespace pt {
    using namespace boost;
} // namespace pt

#endif
