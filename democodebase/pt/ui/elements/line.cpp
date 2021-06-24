#include "line.h"
#include "../draw_event.h"
#include "../event_with_canvas.h"

namespace pt { namespace ui { namespace elements {

line::line()
{
}

line::line(vector2f const& from, vector2f const& to)
:   m_from(from)
,   m_to(to)
{
}

line::line(vector2f const& from, vector2f const& to, box2i const& bounds)
:   bounded_element(bounds)
,   m_from(from)
,   m_to(to)
{
}

void line::set_from(vector2f const& from)
{
    m_from = from;
}

void line::set_to(vector2f const& to)
{
    m_to = to;
}

void line::set_color(vector4f const& color)
{
    m_color = color;
}

void line::handle_event(event& event)
{
    ui::draw_event* p = dynamic_cast<ui::draw_event*>(&event);

    if (!p)
        return;

    draw_line(*p, m_from, m_to, m_color);
}

void line::do_parse(config_node const&)
{
}

void line::do_invalidate()
{
}

}}} // namespace pt::ui::elements
