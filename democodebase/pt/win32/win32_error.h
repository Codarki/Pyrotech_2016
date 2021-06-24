#pragma once
#ifndef PT_WIN32_WIN32_ERROR_H
#define PT_WIN32_WIN32_ERROR_H

#include "pt/exceptions/contextual_exception.h"

namespace pt { namespace win32 {

/// Exception class for windows errors. Uses GetLastError() to get detailed
/// Windows error message.
class win32_error : public exceptions::contextual_exception
{
public:
    explicit win32_error(string_wrapper const& text);
    explicit win32_error(format const& fmt);
    explicit win32_error(char const* text);

    win32_error(win32_error const& other);
    ~win32_error();
};

}} // namespace pt::win32

#endif
