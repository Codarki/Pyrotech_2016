#include "fmod_error.h"
#include "pt/debug_only.h"
#include "pt/win32.h"
#include "pt/boost/format.h"
#include "pt/exceptions/string_context.h"

#include "SDK/fmod/win32/api/inc/fmod_errors.h"

namespace fmod_impl {

fmod_error::fmod_error(FMOD_RESULT result, char const* function)
{
    char const* error_string = FMOD_ErrorString(result);

    std::string error_text = boost::str(boost::format(
        "FMOD error %1% (%2%) %3%") % function % result % error_string);

    attach_context(new pt::exceptions::string_context(error_text));
}

} // namespace fmod_impl
