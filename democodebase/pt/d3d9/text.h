#ifndef PT_D3D9_TEXT_H
#define PT_D3D9_TEXT_H

#include "font.h"
#include "pt/vector.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {

class caret
{
public:
    caret(vector2i position);

private: // data members
    vector2i m_position;
};

/// Abstract base class for source elements for text_area.
class text_source
{
public:
    virtual ~text_source();

public:
    virtual std::string const& text() const = 0;
    virtual d3d9::font const* font() const = 0;
};

/// User settable text source.
class basic_text_source : public text_source
{
public:
    basic_text_source(std::string const& text, d3d9::font const* font,
        vector4f color);
    basic_text_source(d3d9::font const* font, vector4f color);

    void set(std::string const& text);

public: // interface required by text_source
    std::string const& text() const;
    d3d9::font const* font() const;

private: // data members
    std::string m_text;
    vector4f m_color;
    d3d9::font const* m_font;
};

///// Area for text.
//class text_area
//{
//public:
//    text_area(font const& default_font);
//
//    void add(shared_ptr<text_source> source);
//    void draw() const;
//
//private: // data members
//    vector4f m_color;
//    caret m_caret;
//    d3d9::font m_default_font;
//
//    std::vector<shared_ptr<text_source> > m_sources;
//};

///// Area for text.
//class row_text_area
//{
//public:
//    text_area(font const& default_font);
//
//    void add(shared_ptr<text_source> source);
//    void draw() const;
//
//private: // data members
//    vector4f m_color;
//    caret m_caret;
//    d3d9::font m_default_font;
//
//    std::vector<shared_ptr<text_source> > m_sources;
//};

}} // namespace pt::d3d9

#endif
