#ifndef PT_PARAMETER_NODE_H
#define PT_PARAMETER_NODE_H

#include "pt/std/memory.h"

namespace pt {

class animatable_value_base;
class parameter_collection;
class string_wrapper;

/// Named parameter with a value and any number of children. Value can be of
/// any type and be animated or constant. This class is much of like
/// config_node, but the value is not kept as a text.
class parameter_node
{
public:
    parameter_node();
    parameter_node(string_wrapper const& name);
    parameter_node(string_wrapper const& name,
        std::shared_ptr<animatable_value_base> value);
    ~parameter_node();

    parameter_node(parameter_node const& other);
    parameter_node(parameter_node&& other);

    parameter_node const& operator=(parameter_node const& other);
    parameter_node const& operator=(parameter_node&& other);

public:
    void set_name(string_wrapper const& name);
    string_wrapper const& name() const;

    void set_value(std::shared_ptr<animatable_value_base> value);
    std::shared_ptr<animatable_value_base const> value() const;
    std::shared_ptr<animatable_value_base> value();

    parameter_collection& children();
    parameter_collection const& children() const;

private: // data members
    class impl;
    std::unique_ptr<impl> m_pimpl;
};

std::shared_ptr<parameter_node const> get_child(string_wrapper const& name,
    parameter_node const& node);

std::shared_ptr<parameter_node const> find_child(string_wrapper const& name,
    parameter_node const& node);


string_wrapper hierarchy_as_readable_text(parameter_node const& node);
string_wrapper hierarchy_as_readable_text(parameter_collection const& parameters);
string_wrapper hierarchy_as_readable_text(parameter_collection const& parameters, size_t indentation);

} // namespace pt

#endif
