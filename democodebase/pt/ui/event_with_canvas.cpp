#include "event_with_canvas.h"

namespace pt { namespace ui {

event_with_canvas::event_with_canvas(ui::canvas& canvas)
:   m_canvas(canvas)
{
}

ui::canvas const& event_with_canvas::canvas() const
{
    return m_canvas;
}

ui::canvas& event_with_canvas::canvas()
{
    return m_canvas;
}

// free functions

void draw_rectangle(event_with_canvas const& event, box2f const& area,
    vector4f const& color)
{
    event.canvas().draw_rectangle(area, color);
}

void draw_rectangle(event_with_canvas const& event, vector2f const& min,
    vector2f const& max, vector4f const& color)
{
    event.canvas().draw_rectangle(min, max, color);
}

void fill_rectangle(event_with_canvas const& event, box2f const& area,
    vector4f const& color)
{
    event.canvas().fill_rectangle(area, color);
}

void fill_rectangle(event_with_canvas const& event, vector2f const& min,
    vector2f const& max, vector4f const& color)
{
    event.canvas().fill_rectangle(min, max, color);
}

void draw_line(event_with_canvas const& event, vector2f const& start,
    vector2f const& end, vector4f const& color)
{
    event.canvas().draw_line(start, end, color);
}

}} // namespace pt::ui
