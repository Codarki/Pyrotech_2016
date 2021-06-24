#include "text_entry.h"
#include "draw_event.h"
#include "input_event.h"
#include "text_entry_callback.h"
#include "pt/d3d9/fill_rectangle.h"
#include "pt/d3d9/font.h"
#include "pt/ui/common.h"
#include "pt/ui/close_event.h"
#include <locale>

namespace pt { namespace ui {

text_entry::text_entry()
:   m_color(32, 32, 32, 255)
,   m_background(192, 192, 192, 255)
,   m_finished(true)
,   m_aborted(false)
,   m_caret(0)
,   m_selection_start(0)
,   m_selection_end(0)
,   m_callback(new text_entry_callback)
{
}

text_entry::text_entry(box2i const& bounds)
:   bounded_element(bounds)
,   m_color(32, 32, 32, 255)
,   m_background(192, 192, 192, 255)
,   m_finished(true)
,   m_aborted(false)
,   m_caret(0)
,   m_selection_start(0)
,   m_selection_end(0)
,   m_callback(new text_entry_callback)
{
}

text_entry::text_entry(box2i const& bounds, std::shared_ptr<d3d9::font const> font,
    std::string const& text)
:   bounded_element(bounds)
,   m_font(font)
,   m_color(32, 32, 32, 255)
,   m_background(192, 192, 192, 255)
,   m_text(text)
,   m_finished(true)
,   m_aborted(false)
,   m_caret(0)
,   m_selection_start(0)
,   m_selection_end(0)
,   m_callback(new text_entry_callback)
{
}

//void text_entry::set_finished_callback(finished_callback const& callback)
//{
//    m_finished_callback = callback;
//}

void text_entry::activate()
{
    m_finished = false;
    m_aborted = false;
}

bool text_entry::finished() const
{
    return m_finished;
}

bool text_entry::aborted() const
{
    return m_aborted;
}

void text_entry::set_text(std::string const& text)
{
    m_text = text;
}

std::string const& text_entry::text() const
{
    return m_text;
}

void text_entry::set_font(std::shared_ptr<d3d9::font const> font)
{
    m_font = font;
}

void text_entry::set_color(vector4f const& color)
{
    m_color = color;
}

void text_entry::set_background(vector4f const& color)
{
    m_background = color;
}

text_entry_callback& text_entry::callback()
{
    return *m_callback;
}

void text_entry::handle_event(event& event)
{
    dispatch(event);
}

void text_entry::handle_character(character_event& event)
{
    if (!std::isprint(event.character(), std::locale()))
        return;

    // Add printable characters.
    if (m_selection_start != m_selection_end)
    {
        size_t start = std::min(m_selection_start, m_selection_end);
        size_t end = std::max(m_selection_start, m_selection_end);
        size_t count = end - start;
        m_text.erase(start, count);
        m_caret -= count;
    }

    m_text.insert(m_caret, 1, event.character());
    m_caret++;
    m_selection_start = m_caret;
    m_selection_end = m_caret;
    invalidate();
    event.consume();
}

void text_entry::handle_close(close_event& event)
{
    event.set_ok_cancel(true);
}

void text_entry::handle_draw(draw_event& event)
{
    if (!m_font)
        return;

    ui::fill_rectangle(event, bounds(), m_background);

    vector2i min = bounds().center() - bounds().half_axis_lengths();;

    RECT rect;
    rect.top = min[1];
    rect.left = min[0];
    rect.bottom = min[1] + bounds().size()[1];
    rect.right = min[0] + bounds().size()[0];

    vector4i text_color = static_vector_cast<int>(m_color);
    d3d9::draw_text(*m_font, m_text, rect, text_color, 0);

    if (m_selection_end != m_selection_start)
    {
        size_t start = std::min(m_selection_start, m_selection_end);
        size_t end = std::max(m_selection_start, m_selection_end);
        size_t count = end - start;

        // find start of selection
        rect.right = rect.left;
        d3d9::draw_text(*m_font, m_text.substr(0, start), rect, text_color,
            DT_CALCRECT);
        vector2i selection_start = min;
        selection_start[0] += rect.right - rect.left;

        // find end of selection
        rect.right = rect.left;
        d3d9::draw_text(*m_font, m_text.substr(start, count), rect, text_color,
            DT_CALCRECT);
        vector2i selection_end = selection_start;
        selection_end[0] += rect.right - rect.left;
        selection_end[1] += bounds().size()[1];

        vector4f color(0, 0, 192, 255);
        ui::fill_rectangle(event, static_vector_cast<float>(selection_start),
            static_vector_cast<float>(selection_end), color);

        rect.left = selection_start[0];
        rect.right = selection_end[0];

        //rect.left = selection_start[0];

        vector4i text_color2(40, 240, 240, 255);
        d3d9::draw_text(*m_font, m_text.substr(start, count), rect,
            text_color2, 0);
    }

    static bool caret_drawn = false;

    if (!m_finished)
    {
        // draw caret.
        if (!caret_drawn)
        {
            caret_drawn = true;
            invalidate();
        }

        rect.right = rect.left;
        d3d9::draw_text(*m_font, m_text.substr(0, m_caret), rect,
            text_color, DT_CALCRECT);

        vector4f caret_color(0, 0, 0, 128);
        vector2f caret_position = static_vector_cast<float>(min);
        caret_position[0] += rect.right - rect.left;
        ui::fill_rectangle(event, caret_position,
            caret_position + vector2f(2, 14), caret_color);
    }
    //else if (caret_drawn)
    //{
    //    caret_drawn = false;
    //    invalidate();
    //}
}

void text_entry::handle_key_down(key_down_event& event)
{
    if (event.key_code() == VK_RETURN)
    {
        m_finished = true;
        m_callback->call_optional_finished_callback(m_text);

        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_ESCAPE)
    {
        m_aborted = true;
        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_BACK && m_caret)
    {
        if (m_selection_start != m_selection_end)
        {
            size_t start = std::min(m_selection_start, m_selection_end);
            size_t end = std::max(m_selection_start, m_selection_end);

            size_t count = end - start;
            m_text.erase(start, count);
            m_caret -= count;
        }
        else
        {
            if (m_caret > 0)
            {
                m_text.erase(m_caret - 1, 1);
                --m_caret;
            }
        }
        m_selection_start = m_caret;
        m_selection_end = m_caret;
        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_DELETE)
    {
        if (m_selection_start != m_selection_end)
        {
            size_t start = std::min(m_selection_start, m_selection_end);
            size_t end = std::max(m_selection_start, m_selection_end);
            size_t count = end - start;
            m_text.erase(start, count);
            m_caret -= count;
        }
        else
        {
            if (m_caret < m_text.size())
                m_text.erase(m_caret, 1);
        }
        m_selection_start = m_caret;
        m_selection_end = m_caret;
        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_HOME)
    {
        m_caret = 0;
        m_selection_start = m_caret;
        if (!ui::is_key_down(VK_SHIFT))
            m_selection_end = m_caret;
        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_END)
    {
        m_caret = m_text.size();
        m_selection_start = m_caret;
        if (!ui::is_key_down(VK_SHIFT))
            m_selection_end = m_caret;
        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_LEFT && m_caret)
    {
        --m_caret;
        m_selection_start = m_caret;
        if (!ui::is_key_down(VK_SHIFT))
            m_selection_end = m_caret;
        invalidate();
        event.consume();
    }
    else if (event.key_code() == VK_RIGHT && m_caret < m_text.size())
    {
        ++m_caret;
        m_selection_start = m_caret;
        if (!ui::is_key_down(VK_SHIFT))
            m_selection_end = m_caret;
        invalidate();
        event.consume();
    }
}

void text_entry::do_parse(config_node const& )
{
}

void text_entry::do_invalidate()
{
    if (parent())
        parent()->invalidate();
}

}} // namespace pt::ui
