#include "string_wrapper.h"

namespace pt {

string_wrapper::string_wrapper()
{
}

string_wrapper::string_wrapper(string_wrapper const& other)
: m_text(other.m_text)
{
}

string_wrapper::string_wrapper(string_wrapper&& other)
: m_text(move(other.m_text))
{
}

string_wrapper::string_wrapper(std::string const& text)
: m_text(text)
{
}

string_wrapper::string_wrapper(std::string&& text)
: m_text(move(text))
{
}

string_wrapper::string_wrapper(const char* text)
: m_text(text)
{
}

string_wrapper const& string_wrapper::operator=(string_wrapper&& other)
{
    m_text = move(other.m_text);
    return *this;
}

string_wrapper const& string_wrapper::operator=(string_wrapper const& other)
{
    m_text = other.m_text;
    return *this;
}

std::string const& string_wrapper::string() const
{
    return m_text;
}

std::string& string_wrapper::string()
{
    return m_text;
}

} // namespace pt
