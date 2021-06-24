#ifndef PT_WIN32_CLIPBOARD_TEXT_H
#define PT_WIN32_CLIPBOARD_TEXT_H

#include "clipboard.h"
#include "pt/std/string.h"

namespace pt { namespace win32 {

/// Clipboard format for text.
class clipboard_text : public clipboard::format
{
public:
    clipboard_text();
    clipboard_text(std::string const& text);

    std::string const& text() const;

public: // interface required by clipboard::format
    void copy() const;
    void paste();
    bool is_available() const;

private: // data members
    std::string m_text;
};

}} // namespace pt::win32

#endif
