#ifndef PT_BOOST_BIND_H
#define PT_BOOST_BIND_H

#include "auto_link.h"

#pragma warning(push)
#pragma warning(disable: 4702)  // unreachable code
#include <boost/bind.hpp>
#pragma warning(pop)

namespace pt {
    using ::boost::bind;
} // namespace pt

#endif
