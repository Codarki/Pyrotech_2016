#ifndef PT_BOOST_SHARED_PTR_H
#define PT_BOOST_SHARED_PTR_H

//#include "../std/iterator.h"

#include "auto_link.h"

//#include <memory>
#include <boost/shared_ptr.hpp>
//#include "null_deleter.h"

namespace pt {
    //using ::boost::shared_ptr;
    using ::boost::static_pointer_cast;
    using ::boost::const_pointer_cast;
    using ::boost::dynamic_pointer_cast;

    // Allows you to write
    // return make_shared_ptr(new some_very_long_typename)
    // rather than
    // return shared_ptr<some_very_long_typename>(new some_very_long_typename)
    template<typename T>
    inline boost::shared_ptr<T> make_boost_shared_ptr(T* ptr)
    {
        return boost::shared_ptr<T>(ptr);
    }
} // namespace pt

#endif
