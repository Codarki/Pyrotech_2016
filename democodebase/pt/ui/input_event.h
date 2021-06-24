#ifndef PT_UI_INPUT_EVENT_H
#define PT_UI_INPUT_EVENT_H

#include "bounded_event.h"

namespace pt { namespace ui {

/// Abstract base class for all UI input events.
class input_event : public bounded_event
{
public:
    input_event(box2i const& bounds);

    /// \return \c true if this event is consumed.
    bool consumed() const;
    /// Sets this event as consumed.
    void consume();

protected: // hook functions
    virtual void on_consumed();

private: // data members
    bool m_consumed;
};

/// Abstract base class for all keyboard event.
class keyboard_event : public input_event
{
public:
    keyboard_event(int repeat_count);

    /// Informs that this event is actually a collapse of multiple same
    /// events. \return The count of collapsed events.
    int repeat_count() const;

    //int key_code() const;

private: // data members
    //int m_key_code;
    int m_repeat_count;
};

/// Key down event.
class key_down_event : public keyboard_event
{
public:
    key_down_event(int key_code, bool was_down_before, int repeat_count);

    /// \return The virtual key code.
    int key_code() const;
    /// \return \c true if key was down before this event.
    bool was_down_before() const;

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);

private: // data members
    int m_key_code;
    bool m_was_down_before;
};

/// Character event.
class character_event : public keyboard_event
{
public:
    character_event(char character, int repeat_count);

    /// \return ASCII character.
    char character() const;

private: // event virtual function implementations
    void do_dispatch(event_handler& handler);

private: // data members
    char m_character;
};

// \file \todo key_up_event

}} // namespace pt::ui

#endif
