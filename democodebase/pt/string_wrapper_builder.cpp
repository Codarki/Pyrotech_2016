#include "string_wrapper_builder.h"
#include "string_wrapper.h"
#include "string_builder.h"
#include "pt/boost/lexical_cast.h"

namespace pt {

class string_wrapper_builder::impl
{
public:
    impl()
    {
    }

    impl(impl const& other)
    : m_builder(other.m_builder)
    {
    }

    impl(string_builder&& b)
    : m_builder(move(b))
    {
    }

    void append(std::string&& text)
    {
        m_builder.append(std::move(text));
    }

    void append(char const* text)
    {
        m_builder.append(text);
    }

    std::string build()
    {
        return m_builder.string();
    }

private: // data members
    string_builder m_builder;
};

string_wrapper_builder::string_wrapper_builder()
: m_impl(new impl)
{
}

string_wrapper_builder::string_wrapper_builder(string_wrapper_builder const& other)
: m_impl(new impl(*other.m_impl))
{
}

string_wrapper_builder::string_wrapper_builder(string_wrapper_builder&& other)
: m_impl(other.m_impl)
{
    // Steal the implementation.
    other.m_impl = 0;
}

string_wrapper_builder::~string_wrapper_builder()
{
    delete m_impl;
}

string_wrapper_builder const& string_wrapper_builder::operator=(string_wrapper_builder&& other)
{
    delete m_impl;

    // Steal the implementation.
    m_impl = other.m_impl;
    other.m_impl = 0;
    return *this;
}

string_wrapper_builder& string_wrapper_builder::append(string_wrapper const& text)
{
    m_impl->append(std::string(text.string()));
    return *this;
}

string_wrapper_builder& string_wrapper_builder::append(string_wrapper&& text)
{
    m_impl->append(std::move(text.string()));
    return *this;
}

string_wrapper_builder& string_wrapper_builder::append(char const* text)
{
    m_impl->append(text);
    return *this;
}

string_wrapper_builder& string_wrapper_builder::operator()(string_wrapper&& text)
{
    m_impl->append(std::move(text.string()));
    return *this;
}

string_wrapper_builder& string_wrapper_builder::operator()(char const* text)
{
    m_impl->append(text);
    return *this;
}

string_wrapper_builder& string_wrapper_builder::operator+(char const* text)
{
    m_impl->append(text);
    return *this;
}

string_wrapper string_wrapper_builder::build()
{
    return string_wrapper(m_impl->build());
}

// free functions

string_wrapper to_string_wrapper(int v)
{
    return boost::lexical_cast<std::string>(v);
}

string_wrapper to_string_wrapper(unsigned int v)
{
    return boost::lexical_cast<std::string>(v);
}

string_wrapper to_string_wrapper(long v)
{
    return boost::lexical_cast<std::string>(v);
}

string_wrapper to_string_wrapper(unsigned long v)
{
    return boost::lexical_cast<std::string>(v);
}

} // namespace pt
