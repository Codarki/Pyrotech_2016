#include "any_error.h"

namespace pt { namespace exceptions {

any_error::any_error(char const* text)
{
    set_text(text);
}

any_error::any_error(string_wrapper const& text)
{
    set_text(text);
}

any_error::any_error(format const& fmt)
{
    set_text(fmt);
}

any_error::any_error(format&& fmt)
{
    set_text(std::move(fmt));
}

}} // namespace pt
