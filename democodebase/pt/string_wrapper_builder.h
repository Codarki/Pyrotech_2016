#pragma once
#ifndef PT_STRING_WRAPPER_BUILDER_H
#define PT_STRING_WRAPPER_BUILDER_H

namespace pt {

class string_wrapper;

class string_wrapper_builder
{
public:
    string_wrapper_builder();
    string_wrapper_builder(string_wrapper_builder const& other);
    string_wrapper_builder(string_wrapper_builder&& other);
    ~string_wrapper_builder();
    string_wrapper_builder const& operator=(string_wrapper_builder&& other);

    string_wrapper_builder& append(string_wrapper const& text);
    string_wrapper_builder& append(string_wrapper&& text);
    string_wrapper_builder& append(char const* text);

    string_wrapper_builder& operator()(string_wrapper&& text);
    string_wrapper_builder& operator()(char const* text);
    template<typename T>
    string_wrapper_builder& operator()(T&& value);

    string_wrapper_builder& operator+(char const* text);
    template<typename T>
    string_wrapper_builder& operator+(T&& value);

    template<typename T>
    string_wrapper_builder& append(T&& value);

    string_wrapper build();

private: // data members
    class impl;
    impl* m_impl;
};

inline string_wrapper_builder&& move(string_wrapper_builder& text)
{
    return static_cast<string_wrapper_builder&&>(text);
}

string_wrapper to_string_wrapper(int v);
string_wrapper to_string_wrapper(unsigned int v);
string_wrapper to_string_wrapper(long v);
string_wrapper to_string_wrapper(unsigned long v);

template<typename T>
string_wrapper_builder& string_wrapper_builder::operator()(T&& value)
{
    append(to_string_wrapper(std::forward<T>(value)));
    return *this;
}

template<typename T>
string_wrapper_builder& string_wrapper_builder::operator+(T&& value)
{
    append(to_string_wrapper(std::forward<T>(value)));
    return *this;
}

template<typename T>
string_wrapper_builder& string_wrapper_builder::append(T&& value)
{
    append(to_string_wrapper(std::forward<T>(value)));
}


} // namespace pt

#endif
