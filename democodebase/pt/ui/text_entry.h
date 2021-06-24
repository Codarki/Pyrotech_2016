#ifndef PT_UI_TEXT_ENTRY_H
#define PT_UI_TEXT_ENTRY_H

#include "bounded_element.h"
#include "event_handler.h"
#include "event.h"
#include "pt/vector.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {
    class font;
}} // namespace pt::dx

namespace pt { namespace ui {

class text_entry_callback;

/// Text entry UI element.
class text_entry : public bounded_element, public event_handler
{
public: // type definitions
    //typedef function<void (std::string const&)> finished_callback;

public:
    text_entry();
    text_entry(box2i const& bounds);
	text_entry(box2i const& bounds, std::shared_ptr<d3d9::font const> font,
        std::string const& text);

    /// Sets the callback which will be called with the text user has typed.
    //void set_finished_callback(finished_callback const& callback);

    void activate();

    /// \return \c true if text feed has finished.
    bool finished() const;
    /// \return \c true if user has aborted.
    bool aborted() const;

    void set_text(std::string const& text);
    /// \return The text which user has typed so far.
    std::string const& text() const;

    void set_font(std::shared_ptr<d3d9::font const> font);
    void set_color(vector4f const& color);
    void set_background(vector4f const& color);

    text_entry_callback& callback();

public: // interface required by ui::event_handler
    void handle_event(event& event);

private: // ui::event_handler virtual function implementations
    void handle_character(character_event& event);
    void handle_close(close_event& event);
    void handle_draw(draw_event& event);
    void handle_key_down(key_down_event& event);

private: // ui::element virtual function implementations
    void do_parse(config_node const& config);

private: // ui::invalidatable virtual function implementations
    void do_invalidate();

private: // helper functions
private: // data members
    std::string m_text;
    std::shared_ptr<d3d9::font const> m_font;
    vector4f m_color;
    vector4f m_background;
    bool m_finished;
    bool m_aborted;

    size_t m_caret;
    size_t m_selection_start;
    size_t m_selection_end;

    std::unique_ptr<text_entry_callback> m_callback;
    //finished_callback m_finished_callback;
};

}} // namespace pt::ui

#endif
