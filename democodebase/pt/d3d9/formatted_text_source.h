#ifndef PT_D3D9_FORMATTED_TEXT_SOURCE_H
#define PT_D3D9_FORMATTED_TEXT_SOURCE_H

#include "text.h"
#include "pt/format.h"
#include "pt/boost/format.h"

namespace pt { namespace d3d9 {

// Forward declaration.
template<typename T,int C>
class formatted_text_source;

/// Text source which will format \c C values.
template<typename T,int C>
class formatted_text_source : public text_source
{
public:
    formatted_text_source();
    void set(boost::format const& fmt);
    void set(size_t index, T value);

public: // interface required by text_source
    std::string const& text() const;
    d3d9::font const* font() const;

private: // data members
    boost::format m_format;
    T m_values[C];
    mutable std::string m_formatted_text;
    vector4f m_color;
    d3d9::font const* m_font;
};

//todo move to .inl
template<typename T,int C>
formatted_text_source<T,C>::formatted_text_source()
: m_font(0)
{
}

template<typename T,int C>
void formatted_text_source<T,C>::set(boost::format const& fmt)
{
    m_format = fmt;
}

template<typename T,int C>
void formatted_text_source<T,C>::set(size_t index, T value)
{
    assert(index < C);
    m_values[index] = value;
}

template<typename T,int C>
std::string const& formatted_text_source<T,C>::text() const
{
    // Create local copy of the format and feed arguments.
    boost::format local = m_format;
    for (int i = 0; i < C; ++i)
        local % m_values[i];

    m_formatted_text = local.str();

    return m_formatted_text;
}

template<typename T,int C>
d3d9::font const* formatted_text_source<T,C>::font() const
{
    return m_font;
}

}} // namespace pt::d3d9

#endif
