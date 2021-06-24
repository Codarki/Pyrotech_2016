#ifndef PT_EXCEPTIONS_CONTEXT_H
#define PT_EXCEPTIONS_CONTEXT_H

#include "pt/std/string.h"

namespace pt { namespace exceptions {

/// Abstract interface for exception contexts.
/// A context describes the context where exception was thrown.
class context
{
public:
    virtual ~context() {};

public: // interface which subclasses must implement
    /// Returns readable text for this context.
    virtual std::string const& text() const = 0;
};

}} // namespace pt::exceptions

#endif
