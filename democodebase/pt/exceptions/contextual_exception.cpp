#include "contextual_exception.h"
#include "context_stack.h"
#include "context.h"
#include "pt/format.h"
#include "pt/string_wrapper.h"

namespace pt { namespace exceptions {

class contextual_exception::impl
{
public:
    void attach_context(std::shared_ptr<context>&& context)
    {
        m_contexts.push_back(std::move(context));
    }

    void set_text(string_wrapper&& text)
    {
        m_text = move(text);
    }

    std::string const& what() const
    {
        // clear
        std::string().swap(m_what);

        // Construct error text and append contexts.
        m_what += m_text.string();
        if (!m_contexts.empty())
            m_what += ": ";

        for (context_stack::const_iterator i = m_contexts.begin();
            i != m_contexts.end(); ++i)
        {
            m_what += (*i)->text();

            if (i + 1 != m_contexts.end())
                m_what += ": ";
        }

        return m_what;
    }

private: // data members
    context_stack m_contexts;
    string_wrapper m_text;
    mutable std::string m_what;
};

contextual_exception::contextual_exception()
: m_impl(new impl)
{
}

contextual_exception::contextual_exception(context* c)
: m_impl(new impl)
{
    m_impl->attach_context(std::shared_ptr<context>(c));
}

contextual_exception::contextual_exception(std::shared_ptr<context> const& c)
: m_impl(new impl)
{
    m_impl->attach_context(std::shared_ptr<context>(c));
}

contextual_exception::contextual_exception(contextual_exception const& other)
: m_impl(new impl(*other.m_impl))
{
}

contextual_exception::~contextual_exception()
{
}

void contextual_exception::attach_context(context* c)
{
    m_impl->attach_context(std::shared_ptr<context>(c));
}

void contextual_exception::attach_context(std::shared_ptr<context> const& c)
{
    m_impl->attach_context(std::shared_ptr<context>(c));
}

void contextual_exception::set_text(string_wrapper const& text)
{
    m_impl->set_text(string_wrapper(text));
}

void contextual_exception::set_text(char const* text)
{
    m_impl->set_text(string_wrapper(text));
}

void contextual_exception::set_text(format const& fmt)
{
    m_impl->set_text(string_wrapper(fmt.string()));
}

void contextual_exception::set_text(format&& fmt)
{
    m_impl->set_text(string_wrapper(fmt.move_string()));
}

char const* contextual_exception::what() const
{
    return m_impl->what().c_str();
}

}} // namespace pt::exceptions
