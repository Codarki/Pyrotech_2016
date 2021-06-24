#pragma once
#ifndef PT_STRING_BUILDER_H
#define PT_STRING_BUILDER_H

#include "pt/std/string.h"
#include "string_wrapper.h"

namespace pt {

class string_builder
{
public:
    string_builder();
    string_builder(string_builder const& other);
    string_builder(string_builder&& other);
    ~string_builder();
    string_builder const& operator=(string_builder&& other);

    string_builder& append(std::string const& text);
    string_builder& append(std::string&& text);
    string_builder& append(char const* text);

    //std::string const& string() const;
    std::string string();

private: // data members
    std::string m_text;
};

inline string_builder&& move(string_builder& text)
{
    return static_cast<string_builder&&>(text);
}

std::string to_string(int x);
std::string to_string(unsigned int x);
std::string to_string(long x);
std::string to_string(unsigned long x);
std::string to_string(float x);
std::string to_string(double x);
std::string const& to_string(std::string const& x);
std::string to_string(char const* const x);

std::string to_lower_case(std::string const& text);

} // namespace pt

#endif
