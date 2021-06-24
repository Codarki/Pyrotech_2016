#ifndef CONTENT_USER_ABORT_H
#define CONTENT_USER_ABORT_H

#include "pt/exceptions/contextual_exception.h"
#include "pt/exceptions/string_context.h"

namespace content {

using namespace pt;

/// Writes all added texts into current rendertarget.
class user_abort : public exceptions::contextual_exception
{
public:
    user_abort()
    :   exceptions::contextual_exception(
            new exceptions::string_context("User abort"))
    {
    }
};

} // namespace content

#endif
