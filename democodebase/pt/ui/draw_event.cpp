#include "draw_event.h"
#include "event_handler.h"

namespace pt { namespace ui {

draw_event::draw_event(ui::canvas& canvas)
:   event_with_canvas(canvas)
{
}

void draw_event::do_dispatch(event_handler& handler)
{
    handler.handle_draw(*this);
}

// Free functions

//draw_event create_draw_event_with_child_area(draw_event const& parent,
//    box2i const& area)
//{
//    vector2i min_child(min_position(area));
//    vector2i max_child(max_position(area));
//
//    // Clamp.
//    min_child[0] = std::max(min_child[0], 0);
//    min_child[1] = std::max(min_child[1], 0);
//    max_child[0] = std::min(max_child[0], parent.context().area.size()[0]);
//    max_child[1] = std::min(max_child[1], parent.context().area.size()[1]);
//
//    draw_event_context new_context(box_from_min_max(min_child, max_child));
//    draw_event e(new_context);
//    return e;
//}

}} // namespace pt::ui
