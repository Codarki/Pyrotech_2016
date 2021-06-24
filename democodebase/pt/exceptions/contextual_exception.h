#ifndef PT_EXCEPTIONS_CONTEXTUAL_EXCEPTION_H
#define PT_EXCEPTIONS_CONTEXTUAL_EXCEPTION_H

#include "pt/std/memory.h"
#include <exception>

namespace pt {
    class string_wrapper;
    class format;
}

namespace pt { namespace exceptions {

class context;

/// Exception that has a \link exceptions::context context \endlink stack.
/// The context stack describes the level of detail where exception was
/// thrown, last element being the most detailed error description.
class contextual_exception : public std::exception
{
protected:
    /// Constructs null exception. Users (subclasses) are required to call
    /// attach_context() before actually throwing this. TODO any way to get
    /// rid of this nicely?
    contextual_exception();

    /// Constructs exception with \a context. Deletion of given pointer is
    /// handled.
    contextual_exception(context* context);
    /// Constructs exception with \a context.
    contextual_exception(std::shared_ptr<context> const& context);

    contextual_exception(contextual_exception const& other);
    ~contextual_exception();

public:
    /// Adds \a context to context stack. Deletion of given pointer is
    /// handled.
    void attach_context(context* context);
    /// Adds \a context to context stack.
    void attach_context(std::shared_ptr<context> const& context);

    void set_text(string_wrapper const& text);
    void set_text(char const* text);
    void set_text(format const& fmt);
    void set_text(format&& fmt);

public: // std::exception virtual function overrides
    char const* what() const;

private: // data members
    class impl;
    std::unique_ptr<impl> m_impl;
};

}} // namespace pt::exceptions

#endif
