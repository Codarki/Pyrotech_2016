#include "config_node.h"
#include "config_node_utility.h"
#include "any_error.h"
#include "boost/lexical_cast.h"
#include "detail/config_node_child_not_found.h"
#include "detail/config_node_parse_failed.h"
#include "log.h"
#include "pt/std/cassert.h"

namespace pt {
namespace detail {
    void throw_parse_failed(config_node const& node,
        std::string const& text)
    {
        throw config_node_parse_failed(node, text);
    }
} // namespace detail

config_node::config_node()
{
}

config_node::config_node(std::string const& key, std::string const& value)
:   m_key(key)
,   m_value(value)
{
}

config_node::config_node(pt::path const& file)
{
    if (file.extension() == ".xml" || file.extension() == ".XML")
    {
        config_node_builder_xml(*this, file);
    }
    else
    {
        throw any_error("unknown config file format");
    }
}

config_node::const_iterator config_node::child(std::string const& name)
    const
{
    const_iterator i = find_child(name);
    if (i != end())
        return i;

    throw detail::config_node_child_not_found(*this, name);
}

config_node::iterator config_node::child(std::string const& name)
{
    iterator i = find_child(name);
    if (i != end())
        return i;

    throw detail::config_node_child_not_found(*this, name);
}

config_node::const_iterator config_node::find_child(std::string const& name)
    const
{
    for (const_iterator i = m_childs.begin(); i != m_childs.end(); ++i)
    {
        if (i->key() == name)
            return i;
    }
    return end();
}

config_node::iterator config_node::find_child(std::string const& name)
{
    for (iterator i = m_childs.begin(); i != m_childs.end(); ++i)
    {
        if (i->key() == name)
            return i;
    }
    return end();
}

config_node::iterator config_node::begin()
{
    return m_childs.begin();
}

config_node::const_iterator config_node::begin() const
{
    return m_childs.begin();
}

config_node::iterator config_node::end()
{
    return m_childs.end();
}

config_node::const_iterator config_node::end() const
{
    return m_childs.end();
}

config_node::iterator config_node::insert_child(config_node const& node)
{
    return m_childs.insert(end(), node);
}

std::string const& config_node::key() const
{
    return m_key;
}

void config_node::set_key(std::string const& key)
{
    m_key = key;
}

void config_node::set_value(std::string const& value)
{
    m_value = value;
}

std::string const& config_node::value() const
{
    return m_value;
}

text_file_position const& config_node::position() const
{
    return m_text_position;
}

void config_node::set_position(text_file_position const& position)
{
    m_text_position = position;
}

// free functions

std::string optional_child_value_text(config_node const& config,
    std::string const& child, std::string const& default_value_text)
{
    config_node::const_iterator i = config.find_child(child);
    
    return i != config.end() ? i->value() : default_value_text;
}

namespace {
    void write_child(config_node& config, std::string const& key,
        format const& value)
    {
        write_child(config, key, value.string());
    }
} // anonymous namespace

void write_child(config_node& config, std::string const& key,
    std::string const& value)
{
    assert(!key.empty());
    config_node child;
    child.set_key(key);
    child.set_value(value);
    config.insert_child(child);
}

void write_child(config_node& config, std::string const& key, bool value)
{
    std::string const text = value ? "true" : "false";
    write_child(config, key, text);
}

void write_child(config_node& config, std::string const& key, int value)
{
    write_child(config, key, format("%1%") % value);
}

void write_child(config_node& config, std::string const& key, float value)
{
    write_child(config, key, format("%1%") % value);
}

void write_child(config_node& config, std::string const& key, double value)
{
    write_child(config, key, format("%1%") % value);
}

void write_child(config_node& config, std::string const& key, char value)
{
    write_child(config, key, format("%1%") % value);
}

void write_child(config_node& config, std::string const& key, size_t value)
{
    write_child(config, key, format("%1%") % value);
}

void parse_from_text(bool& value, std::string const& text)
{
    value = (text == "true");
}

void parse_from_text(int& value, std::string const& text)
{
    value = lexical_cast<int>(text);
}

void parse_from_text(float& value, std::string const& text)
{
    value = lexical_cast<float>(text);
}

void parse_from_text(double& value, std::string const& text)
{
    value = lexical_cast<double>(text);
}

void parse_from_text(char& value, std::string const& text)
{
    assert(text.length() > 0);
    value = text[0];
}

void parse_from_text(std::string& value, std::string const& text)
{
    value = text;
}

void parse_from_text(size_t& value, std::string const& text)
{
    value = lexical_cast<size_t>(text);
}

} // namespace pt

