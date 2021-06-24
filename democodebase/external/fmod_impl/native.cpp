#include "native.h"
#include "fmod_error.h"
#include "pt/any_error.h"
#include "pt/log.h"
#include "pt/boost/format.h"

#pragma comment(lib, "fmodex_vc.lib")
//#pragma comment(lib, "fmodexp_vc.lib")

namespace fmod_impl {

using namespace pt;

void check_fmod(FMOD_RESULT result, char const* function)
{
    if (result != FMOD_OK)
        throw fmod_error(result, function);
}

} // namespace fmod_impl
