#ifndef EXTERNAL_FMOD_IMPL_NATIVE_H
#define EXTERNAL_FMOD_IMPL_NATIVE_H

#include "SDK/fmod/win32/api/inc/fmod.hpp"

namespace boost {}
namespace pt {}

/// FMOD implementation
namespace fmod_impl {

using namespace pt;

void check_fmod(FMOD_RESULT result, char const* function);

} // namespace fmod_impl

#endif