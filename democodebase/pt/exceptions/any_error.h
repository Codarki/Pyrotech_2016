#pragma once
#ifndef PT_EXCEPTIONS_ANY_ERROR_H
#define PT_EXCEPTIONS_ANY_ERROR_H

#include "contextual_exception.h"
#include "pt/string_wrapper_builder.h"

namespace pt {
    class format;
}

namespace pt { namespace exceptions {

/// Exception class for uncategorized errors.
class any_error : public contextual_exception
{
public:
    any_error(char const* text);
    any_error(string_wrapper const& text);
    any_error(format const& fmt);
    any_error(format&& fmt);

    template<typename T0, typename T1, typename T2>
    any_error(T0&& a0, T1&& a1, T2&& a2)
    {
        string_wrapper_builder builder;

        builder
            .append(std::forward<T0>(a0))
            .append(std::forward<T1>(a1))
            .append(std::forward<T2>(a2));

        set_text(builder.build());
    }
};

}} // namespace pt::exceptions

#endif
