#ifndef PT_UI_MOUSE_EVENT_H
#define PT_UI_MOUSE_EVENT_H

#include "input_event.h"
#include "pt/vector.h"
#include "pt/std/list.h"
#include "pt/std/memory.h"

namespace pt { namespace ui {

enum mouse_button_type
{
    mouse_button_left,
    mouse_button_right,
    mouse_button_middle,
};

/// Abstract base class for all mouse events.
///
/// Child events are useful when the position is in elements own coordinate
/// space, and when passing event to child elements the mouse position needs to
/// be adjusted accordingly. Logic for the child event positions is in the
/// event handling classes.
///
/// Setting the consumed state in any of the child events, will propagate all
/// the way to the root mouse event.
class mouse_event : public input_event
{
public:
    mouse_event(box2i const& bounds, vector2i const& position);

    void set_position(vector2i const& position);
    /// \return Mouse position.
    vector2i const& position() const;

    /// Sets this \a parent for this event. This is used for passing consumed
    /// state all the way back to the root.
    void set_parent(mouse_event* parent);

    /// \return New child mouse event of same type, but new position.
    mouse_event& child_event(vector2i const& position);

private: // type definitions
	typedef std::list<std::shared_ptr<mouse_event> > child_list;

private: // interface which subclasses must implement
    virtual std::shared_ptr<mouse_event> create_instance() = 0;

private: // input_event virtual function overrides
    void on_consumed();

private: // data members
    mouse_event* m_parent;
    child_list m_child_events;
    vector2i m_position;
};

/// Base class for all mouse button events.
class mouse_button_event : public mouse_event
{
public:
    mouse_button_event(box2i const& bounds, vector2i const& position,
        mouse_button_type button, bool is_down);

    void set_doubleclicked();

    mouse_button_type button() const;
    bool is_down() const;
    bool is_double_clicked() const;

private: // data members
    mouse_button_type m_button;
    bool m_is_down;
    bool m_is_double_clicked;
};

/// Event for mouse button down.
class mouse_button_down_event : public mouse_button_event
{
public:
    mouse_button_down_event(box2i const& bounds, vector2i const& position,
        mouse_button_type button);

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);

private: // interface required by mouse_event
    std::shared_ptr<mouse_event> create_instance();
};

/// Event for mouse button up.
class mouse_button_up_event : public mouse_button_event
{
public:
    mouse_button_up_event(box2i const& bounds, vector2i const& position,
        mouse_button_type button);

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);

private: // interface required by mouse_event
    std::shared_ptr<mouse_event> create_instance();
};

/// Event for mouse move.
class mouse_move_event : public mouse_event
{
public:
    mouse_move_event(box2i const& bounds, vector2i const& position);

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);

private: // interface required by mouse_event
    std::shared_ptr<mouse_event> create_instance();
};

}} // namespace pt::ui

#endif
