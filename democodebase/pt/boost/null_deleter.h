#ifndef PT_NULL_DELETER_H
#define PT_NULL_DELETER_H

#include "auto_link.h"

namespace pt {

/// Provides deleter to boost::shared_ptr that does nothing.
class smart_ptr_deleter
{
public:
    void operator()(void*)
    {
    }
};

/// Global access for null deleter for smart pointers.
extern smart_ptr_deleter null_deleter;

} // namespace pt

#endif
