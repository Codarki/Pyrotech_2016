#pragma once
#ifndef PT_FORMAT_H
#define PT_FORMAT_H

#include "pt/string_wrapper.h"
#include "pt/string_builder.h"

namespace pt {

/// String formatting.
/// Same basic syntax as in boost::format. %1%, %2% etc.
class format
{
public:
    format();
    explicit format(char const* text);
    explicit format(std::string const& text);
    explicit format(std::string&& text);
    explicit format(format&& other);
	format(format const& other);
    ~format();
    format const& operator=(format&& other);

    std::string const& string() const;
    pt::string_wrapper string_wrapper() const;

    // Moves the contained string, rendering this class unusable (empty).
    std::string move_string();
    pt::string_wrapper move_string_wrapper();

    template<typename T>
    format& operator%(T const& x);

    void feed_parameter(std::string const& parameter);

private: // data members
    std::string m_text;
    size_t m_fed_paramater_count;
};

inline format&& move(format& fmt)
{
    return static_cast<format&&>(fmt);
}

template<typename T>
format& format::operator%(T const& x)
{
    feed_parameter(to_string(x));
    return *this;
}

} // namespace pt

#endif
