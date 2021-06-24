#include "input_event.h"
#include "event_handler.h"

namespace pt { namespace ui {

// input_event

input_event::input_event(box2i const& bounds)
:   bounded_event(bounds)
,   m_consumed(false)
{
}

bool input_event::consumed() const
{
    return m_consumed;
}

void input_event::consume()
{
    m_consumed = true;
    on_consumed();
}

void input_event::on_consumed()
{
}

// keyboard_event

keyboard_event::keyboard_event(int repeat_count)
:   input_event(box2i())
,   m_repeat_count(repeat_count)
{
}

int keyboard_event::repeat_count() const
{
    return m_repeat_count;
}

// key_down_event

key_down_event::key_down_event(int key_code, bool was_down_before,
    int repeat_count)
:   keyboard_event(repeat_count)
,   m_key_code(key_code)
,   m_was_down_before(was_down_before)
{
}

int key_down_event::key_code() const
{
    return m_key_code;
}

bool key_down_event::was_down_before() const
{
    return m_was_down_before;
}

void key_down_event::do_dispatch(event_handler& handler)
{
    handler.handle_key_down(*this);
}

// character_event

character_event::character_event(char character, int repeat_count)
:   keyboard_event(repeat_count)
,   m_character(character)
{
}

char character_event::character() const
{
    return m_character;
}

void character_event::do_dispatch(event_handler& handler)
{
    handler.handle_character(*this);
}

}} // namespace pt::ui
