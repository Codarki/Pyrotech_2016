#include "error.h"
#include "native.h"
#include "pt/string_wrapper.h"
#include "pt/format.h"
#include "pt/exceptions/contextual_exception.h"
#include "pt/exceptions/string_context.h"

#include <dxerr.h>

#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

namespace pt { namespace d3d9 {

error::error(HRESULT hr, char const* text)
{
    char const* error_name = DXGetErrorString(hr);
    char const* error_desc = DXGetErrorDescription(hr);

    set_text(format("%1% %2% (%3%): %4%")
        % text % error_name % hr % error_desc);
}

compilation_error::compilation_error(string_wrapper const& text)
{
    set_text(text);
}

compilation_error::compilation_error(char const* text)
{
    set_text(string_wrapper(text));
}

void check_dx(HRESULT hr, char const* function)
{
    if (hr != D3D_OK)
        throw error(hr, function);
}

}} // namespace pt::d3d9
