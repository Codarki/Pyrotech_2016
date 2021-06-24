#pragma once
#ifndef PT_STRING_WRAPPER_H
#define PT_STRING_WRAPPER_H

#include "pt/std/string.h"

namespace pt {

class string_wrapper
{
public:
    string_wrapper();
    string_wrapper(string_wrapper const& other);
    string_wrapper(string_wrapper&& other);
    string_wrapper(std::string const& text);
    string_wrapper(std::string&& text);
    string_wrapper(const char* text);

    string_wrapper const& operator=(string_wrapper&& other);
    string_wrapper const& operator=(string_wrapper const& other);

    std::string const& string() const;
    std::string& string();

    char const* c_str() const
    {
        return m_text.c_str();
    }

private: // data members
    std::string m_text;
};

// operator ==
inline bool operator==(string_wrapper const& lhs, string_wrapper const& rhs)
{
    return lhs.string() == rhs.string();
}

inline bool operator==(string_wrapper const& lhs, std::string const& rhs)
{
    return lhs.string() == rhs;
}

inline bool operator==(string_wrapper const& lhs, char const* rhs)
{
    return lhs.string() == rhs;
}

inline bool operator==(std::string const& lhs, string_wrapper const& rhs)
{
    return lhs == rhs.string();
}

inline bool operator==(char const* lhs, string_wrapper const& rhs)
{
    return lhs == rhs.string();
}

// operator !=
inline bool operator!=(string_wrapper const& lhs, string_wrapper const& rhs)
{
    return lhs.string() != rhs.string();
}

inline bool operator!=(string_wrapper const& lhs, std::string const& rhs)
{
    return lhs.string() != rhs;
}

inline bool operator!=(string_wrapper const& lhs, char const* rhs)
{
    return lhs.string() != rhs;
}

inline bool operator!=(std::string const& lhs, string_wrapper const& rhs)
{
    return lhs == rhs.string();
}

inline bool operator!=(char const* lhs, string_wrapper const& rhs)
{
    return lhs == rhs.string();
}

// operator +
inline string_wrapper operator+(string_wrapper const& lhs, string_wrapper const& rhs)
{
    return string_wrapper(lhs.string() + rhs.string());
}

inline string_wrapper operator+(string_wrapper const& lhs, std::string const& rhs)
{
    return string_wrapper(lhs.string() + rhs);
}

inline string_wrapper operator+(string_wrapper const& lhs, char const* rhs)
{
    return string_wrapper(lhs.string() + rhs);
}

inline string_wrapper operator+(char const* lhs, string_wrapper const& rhs)
{
    return string_wrapper(lhs + rhs.string());
}

inline string_wrapper operator+(std::string const& lhs, string_wrapper const& rhs)
{
    return string_wrapper(lhs + rhs.string());
}

//

inline std::string to_string(string_wrapper const& x)
{
    return x.string();
}

inline string_wrapper&& move(string_wrapper& text)
{
    return static_cast<string_wrapper&&>(text);
}

} // namespace pt

#endif
