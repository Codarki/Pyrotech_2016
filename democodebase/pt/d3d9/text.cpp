#include "text.h"

namespace pt { namespace d3d9 {

// caret

caret::caret(vector2i position)
:   m_position(position)
{
}

// text_source

text_source::~text_source()
{
}

// basic_text_source

basic_text_source::basic_text_source(std::string const& text,
    d3d9::font const* font, vector4f color)
:   m_text(text)
,   m_font(font)
,   m_color(color)
{
}

basic_text_source::basic_text_source(d3d9::font const* font, vector4f color)
:   m_font(font)
,   m_color(color)
{
}

void basic_text_source::set(std::string const& text)
{
    m_text = text;
}

std::string const& basic_text_source::text() const
{
    return m_text;
}

d3d9::font const* basic_text_source::font() const
{
    return m_font;
}

//// text_area
//
//text_area::text_area(font const& default_font)
//:   m_default_font(default_font)
//{
//}
//
//void text_area::add(shared_ptr<text_source> source)
//{
//    m_sources.push_back(source);
//}
//
//void text_area::draw() const
//{
//    for (std::vector<shared_ptr<text_source> >::const_iterator i
//        = m_sources.begin(); i != m_sources.end(); ++i)
//    {
//    }
//}

}} // namespace pt::d3d9
