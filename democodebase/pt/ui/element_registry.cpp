#include "element_registry.h"
#include "element_type.h"
#include "pt/any_error.h"
#include "pt/log.h"
#include "pt/format.h"

namespace pt { namespace ui {
namespace {
    element_registry g_singleton_registry;
} // anonymous namespace

element_registry& element_registry::instance()
{
    return g_singleton_registry;
}

element_registry::~element_registry()
{
    if (!m_types.empty())
    {
        std::string type_list = "Element types still registered:\n\n";

        for (container::const_iterator i = m_types.begin(); i != m_types.end();
            ++i)
        {
            type_list += (*i)->name();

            if (i + 1 != m_types.end())
                type_list += "\n";
        }
        pt::log(type_list);
        // can not throw since we are destroying singleton, no catch clause.
        //throw any_error(type_list);
    }
}

void element_registry::add(boost::shared_ptr<element_type> const& type)
{
    for (container::const_iterator i = m_types.begin(); i != m_types.end(); ++i)
    {
        if ((*i)->name() == type->name())
            throw any_error(format("Element type \"%1%\" already registered")
                % type->name());
    }

    m_types.push_back(type);
}

void element_registry::remove(boost::shared_ptr<element_type> const& type)
{
    assert(type);
    container::iterator i = find(type->name(), true);
    m_types.erase(i);
}

boost::shared_ptr<element> element_registry::create(std::string const& name) const
{
    container::const_iterator i = find(name, true);
    return (*i)->create();
}

boost::shared_ptr<element> element_registry::create_optional(std::string const& name) const
{
    container::const_iterator i = find(name, false);
	return (i != m_types.end()) ? (*i)->create() : boost::shared_ptr<element>();
}

element_registry::container::iterator element_registry::find(
    std::string const& name, bool throw_if_not_found)
{
    for (container::iterator i = m_types.begin(); i != m_types.end(); ++i)
    {
        if ((*i)->name() == name)
            return i;
    }

    if (throw_if_not_found)
        throw any_error(format("Element type \"%1%\" not found") % name);

    return m_types.end();
}

element_registry::container::const_iterator element_registry::find(
    std::string const& name, bool throw_if_not_found) const
{
    for (container::const_iterator i = m_types.begin(); i != m_types.end(); ++i)
    {
        if ((*i)->name() == name)
            return i;
    }

    if (throw_if_not_found)
        throw any_error(format("Element type \"%1%\" not found") % name);

    return m_types.end();
}

}} // namespace pt::ui
