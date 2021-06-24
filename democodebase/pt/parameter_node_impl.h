#ifndef PT_PARAMETER_NODE_IMPL_H
#define PT_PARAMETER_NODE_IMPL_H

#include "parameter_node.h"
#include "parameter_collection.h"
#include "pt/string_wrapper.h"

namespace pt {

class parameter_node::impl
{
public:
    impl()
    {
    }

    impl(string_wrapper&& name)
    : m_name(move(name))
    {
    }

    impl(string_wrapper&& name, std::shared_ptr<animatable_value_base>&& value)
    : m_name(move(name))
    , m_value(std::move(value))
    {
    }

    void set_name(string_wrapper&& name)
    {
        m_name = std::move(name);
    }

    string_wrapper const& name() const
    {
        return m_name;
    }

    void set_value(std::shared_ptr<animatable_value_base>&& value)
    {
        m_value = std::move(value);
    }

    std::shared_ptr<animatable_value_base const> value() const
    {
        return m_value;
    }

    std::shared_ptr<animatable_value_base> value()
    {
        return m_value;
    }

    parameter_collection& children()
    {
        return m_children;
    }

    parameter_collection const& children() const
    {
        return m_children;
    }

private: // data members
    string_wrapper m_name;
    parameter_collection m_children;
    std::shared_ptr<animatable_value_base> m_value;
};

} // namespace pt

#endif
