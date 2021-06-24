#ifndef CONTENT_INFO_DISPLAY_H
#define CONTENT_INFO_DISPLAY_H

#include "pt/boost/noncopyable.h"
#include "pt/vector.h"
#include "pt/d3d9/font.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {
    class text_source;
}}

namespace content {

using namespace pt;

/// Writes all added texts into current rendertarget.
class info_display : boost::noncopyable
{
public:
    info_display(d3d9::graphics_device& device, vector2i resolution);

    void set_default_font(d3d9::font const& font);
    void set_default_color(vector4f color);
	void add(std::shared_ptr<d3d9::text_source const> source);

    void draw() const;

private: // type definitions
    typedef std::vector<std::shared_ptr<d3d9::text_source const> > sources;

private: // helper functions

private: // data members
    vector4f m_color;
    //caret m_caret;
    d3d9::font m_default_font;
    sources m_sources;
    vector2i m_resolution;
};

} // namespace content

#endif
