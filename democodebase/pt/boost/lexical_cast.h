#ifndef PT_BOOST_LEXICAL_CAST_H
#define PT_BOOST_LEXICAL_CAST_H

#include "auto_link.h"

#pragma warning(push)
#pragma warning(disable: 4100)  // unreferenced formal parameter
#pragma warning(disable: 4267)  // conversion from 'x' to 'y', possible loss of data
#pragma warning(disable: 4244)  // conversion from 'x' to 'y', possible loss of data
#pragma warning(disable: 4512)  // assignment operator could not be generated
#pragma warning(disable: 4511)  // copy constructor could not be generated
#pragma warning(disable: 4701)  // local variable 'result' may be used without having been initialized
#pragma warning(disable: 4702)  // unreachable code
#pragma warning(disable: 4127)  // conditional expression is constant

#include <boost/lexical_cast.hpp>

#pragma warning(pop)

namespace pt {
    using ::boost::lexical_cast;
} // namespace pt

#endif
