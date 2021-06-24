#ifndef PT_DETAIL_CONFIG_NODE_CHILD_NOT_FOUND_H
#define PT_DETAIL_CONFIG_NODE_CHILD_NOT_FOUND_H

#include "config_node.h"
#include "pt/format.h"
#include "pt/exceptions/text_file_error.h"

namespace pt { namespace detail {

// helper to keep config_node_child_not_found class clean.
inline format child_not_found_message(std::string const& name)
{
    return format("child \"%1%\" not found") % name;
}

/// Invalid child exception.
class config_node_child_not_found : public exceptions::text_file_error
{
public:
    config_node_child_not_found(config_node const& node,
        std::string const& child_name)
    : exceptions::text_file_error(node.position(),
            child_not_found_message(child_name))
    {
    }
};

}} // namespace pt::detail

#endif
