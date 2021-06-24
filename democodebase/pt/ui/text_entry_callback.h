#ifndef PT_UI_TEXT_ENTRY_CALLBACK_H
#define PT_UI_TEXT_ENTRY_CALLBACK_H

#include "pt/boost/function.h"
#include "pt/std/string.h"

namespace pt { namespace ui {

/// Text entry UI element.
class text_entry_callback
{
public: // type definitions
    typedef function<void (std::string const&)> finished_callback;

public:
    /// Sets the callback which will be called with the text user has typed.
    void set_finished_callback(finished_callback const& callback)
    {
        m_finished_callback = callback;
    }

    void call_optional_finished_callback(std::string const& text) const
    {
        if (m_finished_callback)
            m_finished_callback(text);
    }

private: // data members
    finished_callback m_finished_callback;
};

}} // namespace pt::ui

#endif
