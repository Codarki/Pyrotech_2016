#ifndef PT_UI_DRAW_EVENT_H
#define PT_UI_DRAW_EVENT_H

#include "event.h"
#include "event_with_canvas.h"

namespace pt { namespace ui {

/// Draw event.
class draw_event : public event, public event_with_canvas
{
public:
    draw_event(ui::canvas& canvas);

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);
};

/// \return Draw event with new context. \a area is in its parents coordinate
/// space. Position (0, 0) relates to minimum corner of the parent. \a area is
/// clamped into parent area.
//draw_event create_draw_event_with_child_area(draw_event const& parent,
//    box2i const& area);

}} // namespace pt::ui

#endif
