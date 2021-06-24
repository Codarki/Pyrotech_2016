#include "info_display.h"
#include "pt/d3d9/text.h"

namespace content {

namespace {
    d3d9::font create_default_font(d3d9::graphics_device& device)
    {
        com_ptr<ID3DXFont> native
            = d3d9::create_d3dx_font(device, "Arial", 14);

        return d3d9::font(device, native);
    }
} // anonymous namespace

info_display::info_display(d3d9::graphics_device& device, vector2i resolution)
:   m_default_font(create_default_font(device))
,   m_resolution(resolution)
,   m_color(1, 1, 1, 1)
{
}

void info_display::set_default_font(d3d9::font const& font)
{
    m_default_font = font;
}

void info_display::set_default_color(vector4f color)
{
    m_color = color;
}

void info_display::add(std::shared_ptr<d3d9::text_source const> source)
{
    assert(source);
    m_sources.push_back(source);
}

void info_display::draw() const
{
    //d3d9::caret caret(vector2i(0, 0));

    vector2i position(0, 0);

    for (sources::const_iterator i = m_sources.begin(); i != m_sources.end();
        ++i)
    {
        d3d9::text_source const& source = **i;
        d3d9::font const* font = source.font();
        if (!font)
            font = &m_default_font;

        box2i area = pt::box_from_min_max(position, position);
        area = d3d9::required_area(*font, source.text(), area);

        vector4i rgba = static_vector_cast<int>(m_color * 255.0f);
        int flags = 0;

        d3d9::draw_text(*font, source.text(), area, rgba, flags);

        position[1] += area.size()[1];
    }
}

} // namespace content
