#include "string_context.h"
#include "pt/format.h"

namespace pt { namespace exceptions {

string_context::string_context(char const* text)
: m_text(text)
{
}

string_context::string_context(std::string const& text)
: m_text(text)
{
}

string_context::string_context(format const& fmt)
: m_text(fmt.string())
{
}

std::string const& string_context::text() const
{
    return m_text;
}

}} // namespace pt::exceptions
