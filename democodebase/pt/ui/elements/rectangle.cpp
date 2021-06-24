#include "rectangle.h"
#include "../draw_event.h"
#include "pt/d3d9/draw_rectangle.h"

namespace pt { namespace ui { namespace elements {

rectangle::rectangle()
{
}

rectangle::rectangle(box2i const& bounds)
:   bounded_element(bounds)
{
}

void rectangle::set_border_color(vector4f const& color)
{
    m_border_color = color;
}

void rectangle::handle_event(event& event)
{
    if (ui::draw_event* p = dynamic_cast<ui::draw_event*>(&event))
    {
        ui::draw_rectangle(*p, bounds(), m_border_color);

        //vector4f background_color(32, 32, 32, 255);
        //d3d9::fill_rectangle(bounds(), background_color);
    }
}

void rectangle::do_parse(config_node const&)
{
}

void rectangle::do_invalidate()
{
}

void rectangle::on_set_bounds()
{
}

}}} // namespace pt::ui::elements
