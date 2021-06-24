#include "mouse_button_event_handler.h"

namespace pt { namespace ui {

class mouse_button_event;
class mouse_button_down_event;
class mouse_button_up_event;

// mouse_button_down_event_handler

void mouse_button_down_event_handler::handle_mouse_button_down(
    mouse_button_down_event&)
{
}

// mouse_button_up_event_handler

void mouse_button_up_event_handler::handle_mouse_button_up(
    mouse_button_up_event&)
{
}

// mouse_button_event_handler

void mouse_button_event_handler::handle_mouse_button(mouse_button_event&)
{
}

}} // namespace pt::ui
