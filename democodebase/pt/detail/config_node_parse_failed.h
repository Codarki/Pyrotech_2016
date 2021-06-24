#ifndef PT_DETAIL_CONFIG_NODE_CHILD_PARSE_FAILED_H
#define PT_DETAIL_CONFIG_NODE_CHILD_PARSE_FAILED_H

#include "config_node.h"
#include "pt/format.h"
#include "pt/exceptions/text_file_error.h"

namespace pt { namespace detail {

/// Failed parse exception.
class config_node_parse_failed : public exceptions::text_file_error
{
public:
    config_node_parse_failed(config_node const& node,
        std::string const& reason)
    : exceptions::text_file_error(node.position(), format(reason))
    {
    }
};

}} // namespace pt::detail

#endif
