#ifndef PT_BOOST_NONCOPYABLE_H
#define PT_BOOST_NONCOPYABLE_H

#include "auto_link.h"

// sucks to disable these globally!
//#pragma warning(disable: 4511)  // copy constructor could not be generated
//#pragma warning(disable: 4512)  // assignment operator could not be generated

#pragma warning(push)
//#pragma warning(disable: 4702)  // unreachable code
#include <boost/noncopyable.hpp>
#pragma warning(pop)

namespace pt {
    //using boost::noncopyable;

    class nonassignable
    {
    protected:
        nonassignable() {}
        ~nonassignable() {}

    private:  // emphasize the following members are private
        nonassignable const& operator=(nonassignable const&);
    };

} // namespace pt

#endif
