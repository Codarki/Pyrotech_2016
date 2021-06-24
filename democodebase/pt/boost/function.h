#ifndef PT_BOOST_FUNCTION_H
#define PT_BOOST_FUNCTION_H

#include "auto_link.h"

#pragma warning(push)
#pragma warning(disable: 4018)  // signed/unsigned mismatch
#pragma warning(disable: 4511)  // copy constructor could not be generated
#pragma warning(disable: 4512)  // assignment operator could not be generated
#pragma warning(disable: 4702)  // unreachable code
#include <boost/function.hpp>
#pragma warning(pop)

namespace pt {
    using ::boost::function;
} // namespace pt

#endif
