#ifndef PT_CONFIG_NODE_UTILITY_H
#define PT_CONFIG_NODE_UTILITY_H

#include "path.h"
#include "config_node_fwd.h"

namespace pt {

/// Builds config_node tree from \a xml file. \relatesalso config_node
void config_node_builder_xml(config_node& node, pt::path const& xml);

void write_xml(pt::path const& xml, config_node const& config);

void write_to_text(std::string& text, config_node const& config);
void create_config_from_text(config_node& node, std::string const& text);
} // namespace pt

#endif
