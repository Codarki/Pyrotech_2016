#include "parameter_node.h"
#include "parameter_node_impl.h"
#include "animatable_value_base.h"
#include "format.h"

namespace pt {

parameter_node::parameter_node()
: m_pimpl(new impl)
{
}

parameter_node::parameter_node(string_wrapper const& name)
: m_pimpl(new impl(string_wrapper(name)))
{
}

parameter_node::parameter_node(string_wrapper const& name,
    std::shared_ptr<animatable_value_base> value)
: m_pimpl(new impl(string_wrapper(name), std::shared_ptr<animatable_value_base>(value)))
{
}

parameter_node::~parameter_node()
{
}

parameter_node::parameter_node(parameter_node const& other)
: m_pimpl(new impl(*other.m_pimpl))
{
}

parameter_node::parameter_node(parameter_node&& other)
: m_pimpl(move(other.m_pimpl))
{
}

parameter_node const& parameter_node::operator=(parameter_node const& other)
{
    m_pimpl.reset(new impl(*other.m_pimpl));
    return *this;
}

parameter_node const& parameter_node::operator=(parameter_node&& other)
{
    m_pimpl = move(other.m_pimpl);
    return *this;
}

void parameter_node::set_name(string_wrapper const& name)
{
    m_pimpl->set_name(string_wrapper(name));
}

string_wrapper const& parameter_node::name() const
{
    return m_pimpl->name();
}

void parameter_node::set_value(std::shared_ptr<animatable_value_base> value)
{
    m_pimpl->set_value(std::move(value));
}

std::shared_ptr<animatable_value_base const> parameter_node::value() const
{
    return m_pimpl->value();
}

std::shared_ptr<animatable_value_base> parameter_node::value()
{
    return m_pimpl->value();
}

parameter_collection& parameter_node::children()
{
    return m_pimpl->children();
}

parameter_collection const& parameter_node::children() const
{
    return m_pimpl->children();
}

// free functions

std::shared_ptr<parameter_node const> get_child(string_wrapper const& name,
    parameter_node const& node)
{
    return *node.children().child(name);
}

std::shared_ptr<parameter_node const> find_child(string_wrapper const& name,
    parameter_node const& node)
{
    return *node.children().find_child(name);
}

void hierarchy_as_readable_text_recursive(parameter_node const& node, size_t indentation, std::string& result)
{
    std::string value_text("no value");
    std::shared_ptr<animatable_value_base const> shared_value = node.value();
    if (shared_value)
    {
        animatable_value_base& value = const_cast<animatable_value_base&>(*shared_value);
        //std::type_info const& info = typeid(value);
        //value_text = info.name();
        value_text = value.type_name().string();
    }

    std::string indentation_text;
    for (size_t i = 0; i < indentation; ++i)
    {
        indentation_text += ' ';
    }

    result += (pt::format("%1%%2% - %3%\r\n") % indentation_text % node.name() % value_text).string();

    for (auto i = node.children().begin(); i != node.children().end(); ++i)
    {
        hierarchy_as_readable_text_recursive(**i, indentation + 2, result);
    }
}

string_wrapper hierarchy_as_readable_text(parameter_node const& node)
{
    std::string result;
    result.reserve(500);

    hierarchy_as_readable_text_recursive(node, 0, result);
    return result;
}

string_wrapper hierarchy_as_readable_text(parameter_collection const& parameters)
{
    return hierarchy_as_readable_text(parameters, 0);
}

string_wrapper hierarchy_as_readable_text(parameter_collection const& parameters, size_t indentation)
{
    std::string result;
    result.reserve(500);

    for (auto i = parameters.begin(); i != parameters.end(); ++i)
    {
        hierarchy_as_readable_text_recursive(**i, indentation, result);
    }

    return result;
}


} // namespace pt
