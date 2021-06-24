#ifndef PT_CONFIG_NODE_INL
#define PT_CONFIG_NODE_INL

namespace pt {
namespace detail{
    // Throws an exception. hides the include dependecies into .cpp
    void throw_parse_failed(config_node const& node,
        std::string const& text);
}

template<typename T>
void config_node::parse(T& result) const
{
    //if (m_value.empty())
    //    detail::throw_parse_failed(*this, "empty value");

    parse_from_text(result, m_value);
}

template<typename T>
void config_node::parse_child(std::string const& name, T& result) const
{
    child(name)->parse(result);
}

template<typename T>
void config_node::parse_optional_child(std::string const& name, T& result)
    const
{
    const_iterator i = find_child(name);
    if (i != end())
        i->parse(result);
}

template<typename T>
void config_node::parse_optional_child(std::string const& name, T& result,
    T const& default_value) const
{
    const_iterator i = find_child(name);
    if (i != end())
        i->parse(result);
    else
        result = default_value;
}

} // namespace pt

#endif
