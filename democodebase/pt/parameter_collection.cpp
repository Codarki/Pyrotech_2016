#include "parameter_collection.h"
#include "parameter_node.h"
#include "parameter_node_utility.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/std/cassert.h"
#include "pt/std/algorithm.h"
#include "pt/std/iterator.h"

namespace pt {

parameter_collection::iterator parameter_collection::begin()
{
    return m_children.begin();
}

parameter_collection::iterator parameter_collection::end()
{
    return m_children.end();
}

parameter_collection::const_iterator parameter_collection::begin() const
{
    return m_children.begin();
}

parameter_collection::const_iterator parameter_collection::end() const
{
    return m_children.end();
}

parameter_collection::const_iterator parameter_collection::child(
    string_wrapper const& name) const
{
    const_iterator i = find_child(name);
    if (i != end())
        return i;

    throw any_error(format("Child parameter \"%1%\" not found.") % name);
}

parameter_collection::const_iterator parameter_collection::find_child(
    string_wrapper const& name) const
{
    for (const_iterator i = begin(); i != end(); ++i)
    {
        std::shared_ptr<parameter_node> const& node = *i;

        if (name == node->name())
            return i;
    }
    return end();
}

void parameter_collection::add(std::shared_ptr<parameter_node> node)
{
    std::back_inserter(m_children) = node;
}

void parameter_collection::remove(std::shared_ptr<parameter_node> node)
{
    parameter_collection::iterator i = find(node);
    assert(i != end());
    m_children.erase(i);
}

size_t parameter_collection::size() const
{
    return m_children.size();
}

bool parameter_collection::empty() const
{
    return m_children.empty();
}

parameter_collection::iterator parameter_collection::find(
    std::shared_ptr<parameter_node> node)
{
    return std::find(begin(), end(), node);
}

std::shared_ptr<parameter_node> find_child(std::string const& name,
    std::string const& value, parameter_collection const& parameters)
{
    for (parameter_collection::const_iterator i = parameters.begin();
        i != parameters.end(); ++i)
    {
        std::shared_ptr<parameter_node> const& node = *i;

        if (name == node->name()
            && value == constant_value<std::string>(*node))
        {
            return node;
        }
    }

    return nullptr;
}

std::shared_ptr<parameter_node> optional_child_node(std::string const& name,
    parameter_collection const& parameters)
{
    parameter_collection::const_iterator i = parameters.find_child(name);
    if (i != parameters.end())
        return *i;

    return nullptr;
}

} // namespace pt
