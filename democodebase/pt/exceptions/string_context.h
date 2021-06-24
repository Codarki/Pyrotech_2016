#ifndef PT_EXCEPTIONS_STRING_CONTEXT_H
#define PT_EXCEPTIONS_STRING_CONTEXT_H

#include "context.h"

namespace pt {
    class format;
}

namespace pt { namespace exceptions {

/// Plain text exception context.
class string_context : public context
{
public:
    /// Constructs string context from \a text.
    string_context(char const* text);
    string_context(std::string const& text);
    string_context(format const& fmt);

public: // interface required by context
    std::string const& text() const;

private: // data members
    std::string m_text;
};

}} // namespace pt::exceptions

#endif
