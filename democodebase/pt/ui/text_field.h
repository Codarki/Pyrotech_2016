#ifndef PT_UI_TEXT_FIELD_H
#define PT_UI_TEXT_FIELD_H

#include "bounded_element.h"
#include "event_handler.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {
    class font;
}} // namespace pt::dx

namespace pt { namespace ui {

/// text_field UI element.
class text_field : public bounded_element, public event_handler
{
public: // type definitions
    enum alignment
    {
        align_left,
        align_right,
        align_center,
    };

public:
    text_field();
    text_field(box2i const& bounds, vector4f const& color);

    /// Sets \a text for this text field. Invalidates itself.
    void set_text(std::string const& text);
    std::string const& text() const;

    /// Sets \a font for this text field. Invalidates itself.
	void set_font(std::shared_ptr<d3d9::font const> font);

    void set_color(vector4f const& color);
    void set_background(vector4f const& color);

    /// Sets the alignment. Default alignment is \c align_left.
    void set_alignment(alignment alignment);

public: // interface required by ui::event_handler
    void handle_event(event& event);

private: // ui::event_handler virtual function implementations
    void handle_draw(draw_event& event);

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

private: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // data members
    std::string m_text;
    vector4f m_color;
    vector4f m_background;
    alignment m_alignment;
    std::shared_ptr<d3d9::font const> m_font;
};

}} // namespace pt::ui

#endif
