#ifndef EXTERNAL_FMOD_IMPL_FMOD_ERROR_H
#define EXTERNAL_FMOD_IMPL_FMOD_ERROR_H

#include "native.h"
#include "pt/exceptions/contextual_exception.h"
#include <boost/format/format_fwd.hpp>

namespace fmod_impl {

/// Exception class for FMOD errors.
class fmod_error : public pt::exceptions::contextual_exception
{
public:
    fmod_error(FMOD_RESULT result, char const* function);
};

} // namespace fmod_impl

#endif
