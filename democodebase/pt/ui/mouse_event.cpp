#include "mouse_event.h"
#include "event_handler.h"

namespace pt { namespace ui {

// mouse_event

mouse_event::mouse_event(box2i const& bounds, vector2i const& position)
:   input_event(bounds)
,   m_position(position)
,   m_parent(0)
{
}

void mouse_event::set_position(vector2i const& position)
{
    m_position = position;
}

vector2i const& mouse_event::position() const
{
    return m_position;
}

void mouse_event::set_parent(mouse_event* parent)
{
    m_parent = parent;
}

mouse_event& mouse_event::child_event(vector2i const& position)
{
	auto new_event = create_instance();
    new_event->set_parent(this);
    new_event->set_position(position);
    m_child_events.push_back(new_event);
    return *new_event;
}

void mouse_event::on_consumed()
{
    if (m_parent)
        m_parent->consume();
}

// mouse_button_event

mouse_button_event::mouse_button_event(box2i const& bounds,
    vector2i const& position, mouse_button_type button, bool is_down)
:   mouse_event(bounds, position)
,   m_button(button)
,   m_is_down(is_down)
,   m_is_double_clicked(false)
{
}

void mouse_button_event::set_doubleclicked()
{
    m_is_double_clicked = true;
}

mouse_button_type mouse_button_event::button() const
{
    return m_button;
}

bool mouse_button_event::is_down() const
{
    return m_is_down;
}

bool mouse_button_event::is_double_clicked() const
{
    return m_is_double_clicked;
}

// mouse_button_down_event

mouse_button_down_event::mouse_button_down_event(box2i const& bounds,
    vector2i const& position, mouse_button_type button)
:   mouse_button_event(bounds, position, button, true)
{
}

std::shared_ptr<mouse_event> mouse_button_down_event::create_instance()
{
    return std::make_shared<mouse_button_down_event>(*this);
}

void mouse_button_down_event::do_dispatch(event_handler& handler)
{
    handler.handle_mouse_button_down(*this);
}

// mouse_button_up_event

mouse_button_up_event::mouse_button_up_event(box2i const& bounds,
    vector2i const& position, mouse_button_type button)
:   mouse_button_event(bounds, position, button, false)
{
}

std::shared_ptr<mouse_event> mouse_button_up_event::create_instance()
{
    return std::make_shared<mouse_button_up_event>(*this);
}

void mouse_button_up_event::do_dispatch(event_handler& handler)
{
    handler.handle_mouse_button_up(*this);
}

// mouse_move_event

mouse_move_event::mouse_move_event(box2i const& bounds,
    vector2i const& position)
:   mouse_event(bounds, position)
{
}

std::shared_ptr<mouse_event> mouse_move_event::create_instance()
{
    return std::make_shared<mouse_move_event>(*this);
}

void mouse_move_event::do_dispatch(event_handler& handler)
{
    handler.handle_mouse_move(*this);
}

}} // namespace pt::ui
