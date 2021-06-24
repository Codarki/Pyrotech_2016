#include "context_stack.h"

namespace pt { namespace exceptions {

bool context_stack::empty() const
{
    return m_contexts.empty();
}

void context_stack::push_back(std::shared_ptr<context>&& context)
{
    m_contexts.push_back(std::move(context));
}

context_stack::const_iterator context_stack::begin() const
{
    return m_contexts.begin();
}

context_stack::const_iterator context_stack::end() const
{
    return m_contexts.end();
}

}} // namespace pt::exceptions
