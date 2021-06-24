#include "button.h"
#include "event.h"

namespace pt { namespace ui {

button::button()
:   bounded_element()
,   m_text_field()
{
    m_text_field.set_parent(this);
}

button::button(box2i const& bounds)
:   bounded_element(bounds)
,   m_text_field(bounds, vector4f(255, 255, 255, 255))
{
    m_text_field.set_parent(this);
}

void button::set_text(std::string const& text)
{
    m_text_field.set_text(text);
}

std::string const& button::text() const
{
    return m_text_field.text();
}

void button::set_font(std::shared_ptr<d3d9::font> font)
{
    m_text_field.set_font(font);
}

void button::handle_event(event& event)
{
    m_text_field.handle_event(event);
}

void button::do_parse(config_node const&)
{
}

void button::do_invalidate()
{
    if (parent())
        parent()->invalidate();
}

void button::on_set_bounds()
{
    m_text_field.set_bounds(bounds());
}

}} // namespace pt::ui
