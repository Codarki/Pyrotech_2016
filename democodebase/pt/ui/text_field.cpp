#include "text_field.h"
#include "draw_event.h"
#include "pt/d3d9/fill_rectangle.h"
#include "pt/d3d9/font.h"

namespace pt { namespace ui {

text_field::text_field()
:   bounded_element()
,   m_alignment(align_left)
{
}

text_field::text_field(box2i const& bounds, vector4f const& color)
:   bounded_element(bounds)
,   m_color(color)
,   m_alignment(align_left)
{
}

void text_field::set_text(std::string const& text)
{
    m_text = text;
    invalidate();
}

std::string const& text_field::text() const
{
    return m_text;
}

void text_field::set_font(std::shared_ptr<d3d9::font const> font)
{
    m_font = font;
    invalidate();
}

void text_field::set_color(vector4f const& color)
{
    m_color = color;
}

void text_field::set_background(vector4f const& color)
{
    m_background = color;
}

void text_field::set_alignment(alignment alignment)
{
    m_alignment = alignment;
}

void text_field::handle_event(event& event)
{
    dispatch(event);
}

void text_field::handle_draw(draw_event& event)
{
    if (!m_font)
        return;

    if (m_background[3] > 0)
        ui::fill_rectangle(event, bounds(), m_background);

    int flags = 0;

    switch(m_alignment)
    {
    case align_left:
        flags |= DT_LEFT;
        break;

    case align_right:
        flags |= DT_RIGHT;
        break;

    case align_center:
        flags |= DT_CENTER;
        break;
    }

    d3d9::draw_text(*m_font, m_text, bounds(),
        static_vector_cast<int>(m_color), flags);
}

void text_field::do_parse(config_node const&)
{
}

void text_field::do_invalidate()
{
    if (parent())
        parent()->invalidate();
}

}} // namespace pt::ui
