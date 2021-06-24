#ifndef PT_BOOST_CAST_H
#define PT_BOOST_CAST_H

#include "auto_link.h"

#include <boost/cast.hpp>

namespace boost {}

namespace pt {
    using ::boost::polymorphic_cast;
    using ::boost::polymorphic_downcast;
    using ::boost::numeric_cast;
} // namespace pt

#endif
