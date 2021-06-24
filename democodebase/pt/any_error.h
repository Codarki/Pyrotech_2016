#ifndef PT_ANY_ERROR_H
#define PT_ANY_ERROR_H

#include "exceptions/any_error.h"

namespace pt {

/// little helper so pt::exceptions::any_error can be used as
/// pt::any_error. \relatesalso exceptions::any_error
typedef pt::exceptions::any_error any_error;

} // namespace pt

#endif
