#include "string_builder.h"
#include "pt/boost/lexical_cast.h"

namespace pt {

string_builder::string_builder()
{
}

string_builder::string_builder(string_builder const& other)
: m_text(other.m_text)
{
}

string_builder::string_builder(string_builder&& other)
: m_text(move(other.m_text))
{
}

string_builder::~string_builder()
{
}

string_builder const& string_builder::operator=(string_builder&& other)
{
    m_text = move(other.m_text);
    return *this;
}

string_builder& string_builder::append(std::string const& text)
{
    m_text += text;
    return *this;
}

string_builder& string_builder::append(std::string&& text)
{
    m_text += move(text);
    return *this;
}

string_builder& string_builder::append(char const* text)
{
    m_text += text;
    return *this;
}

//std::string const& string_builder::string() const
//{
//    return m_text;
//}

std::string string_builder::string()
{
    return std::move(m_text);
}

// free functions

std::string to_string(int x)
{
    return boost::lexical_cast<std::string>(x);
}

std::string to_string(unsigned int x)
{
    return boost::lexical_cast<std::string>(x);
}

std::string to_string(long x)
{
    return boost::lexical_cast<std::string>(x);
}

std::string to_string(unsigned long x)
{
    return boost::lexical_cast<std::string>(x);
}

std::string to_string(float x)
{
    return boost::lexical_cast<std::string>(x);
}

std::string to_string(double x)
{
    return boost::lexical_cast<std::string>(x);
}

std::string const& to_string(std::string const& x)
{
    return x;
}

std::string to_string(char const* const x)
{
    return std::string(x);
}

std::string to_lower_case(std::string const& text)
{
    auto result = text;
    std::transform(text.begin(), text.end(), result.begin(), ::tolower);
    return result;
}

} // namespace pt
