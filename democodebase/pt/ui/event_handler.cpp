#include "event_handler.h"
#include "event.h"

namespace pt { namespace ui {

event_handler::~event_handler()
{
}

void event_handler::dispatch(event& event)
{
    event.dispatch(*this);
}

//void event_handler::handle_character(character_event&)
//{
//}
//
//void event_handler::handle_close(close_event&)
//{
//}
//
//void event_handler::handle_draw(draw_event&)
//{
//}
//
//void event_handler::handle_key_down(key_down_event&)
//{
//}
//
//void event_handler::handle_mouse_button_down(mouse_button_down_event&)
//{
//}
//
//void event_handler::handle_mouse_button_up(mouse_button_up_event&)
//{
//}
//
//void event_handler::handle_mouse_move(mouse_move_event&)
//{
//}

}} // namespace pt::ui
