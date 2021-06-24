#ifndef PT_EXCEPTIONS_CONTEXT_STACK_H
#define PT_EXCEPTIONS_CONTEXT_STACK_H

#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace exceptions {

class context;

class context_stack
{
private: // type definitions
    typedef std::vector<std::shared_ptr<context> > container;

public:
    typedef container::const_iterator const_iterator;

    bool empty() const;
    void push_back(std::shared_ptr<context>&& context);
    const_iterator begin() const;
    const_iterator end() const;

private: // data members
    container m_contexts;
};

}} // namespace pt::exceptions

#endif
