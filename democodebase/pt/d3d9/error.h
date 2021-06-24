#ifndef PT_D3D9_ERROR_H
#define PT_D3D9_ERROR_H

#include "pt/win32.h"
#include "pt/exceptions/contextual_exception.h"

namespace pt { namespace d3d9 {

/// Exception for DirectX errors.
class error : public exceptions::contextual_exception
{
public:
    error(HRESULT hr, char const* text);
};

/// Exception for DirectX shader compilation errors.
class compilation_error : public exceptions::contextual_exception
{
public:
    compilation_error(string_wrapper const& text);
    compilation_error(char const* text);
};

/// Throws exception if \a hr is anything but D3D_OK.
void check_dx(HRESULT hr, char const* function);

}} // namespace pt::d3d9

#endif
