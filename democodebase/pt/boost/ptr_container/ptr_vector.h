#ifndef PT_BOOST_PTR_CONTAINER_H
#define PT_BOOST_PTR_CONTAINER_H

#include "../auto_link.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4511) // copy constructor could not be generated
#pragma warning(disable: 4512) // assignment operator could not be generated
#pragma warning(disable: 4927) // illegal conversion; more than one user-defined conversion has been implicitly applied
#include <boost/ptr_container/ptr_vector.hpp>

#pragma warning(pop)


namespace pt {
    using boost::ptr_vector;

    // boost::ptr_vector<T,boost::view_clone_allocator>

} // namespace pt

#endif
