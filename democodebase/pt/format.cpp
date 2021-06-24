#include "format.h"
#include "string_builder.h"
#include "pt/exceptions/contextual_exception.h"
#include "pt/boost/lexical_cast.h"

namespace pt {

class format_argument_error : public pt::exceptions::contextual_exception
{
public:
    format_argument_error(char const* text)
    {
        set_text(text);
    }
};

format::format()
: m_fed_paramater_count(0)
{
}

format::format(char const* text)
//: m_text(text)
: m_fed_paramater_count(0)
{
    size_t character_count = std::strlen(text);
    m_text.reserve(character_count + 40);
    m_text.assign(text, character_count);
}

format::format(std::string const& text)
: m_text(text)
, m_fed_paramater_count(0)
{
}

format::format(std::string&& text)
: m_text(std::move(text))
, m_fed_paramater_count(0)
{
}

format::format(format&& other)
: m_text(std::move(other.m_text))
, m_fed_paramater_count(other.m_fed_paramater_count)
{
}

format::format(format const& other)
: m_text(other.m_text)
, m_fed_paramater_count(other.m_fed_paramater_count)
{
}

format::~format()
{
}

format const& format::operator=(format&& other)
{
    m_text = std::move(other.m_text);
    m_fed_paramater_count = other.m_fed_paramater_count;
    return *this;
}

std::string const& format::string() const
{
    return m_text;
}

pt::string_wrapper format::string_wrapper() const
{
    return pt::string_wrapper(m_text);
}

std::string format::move_string()
{
    return std::move(m_text);
}

pt::string_wrapper format::move_string_wrapper()
{
    pt::string_wrapper wrapper(std::move(m_text));
    return pt::move(wrapper);
}

void format::feed_parameter(std::string const& parameter)
{
    ++m_fed_paramater_count;

    std::string marker;
    marker.reserve(3);
    marker += '%' + boost::lexical_cast<std::string>(m_fed_paramater_count) + '%'; 

    std::string::size_type position = m_text.find(marker);
    if (position == std::string::npos)
        throw format_argument_error("Feeding too many arguments");

    while (position != std::string::npos)
    {
        position = m_text.find(marker, position);

        if (position != std::string::npos)
        {
            m_text.replace(position, marker.length(), parameter);
            position += parameter.length() - marker.length();
        }
    }
}

// free functions

} // namespace pt
