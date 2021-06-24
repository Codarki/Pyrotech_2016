#pragma once
#ifndef PT_D3D9_RENDERING_PARAMETER_NODE_SETTER_CREATE_H
#define PT_D3D9_RENDERING_PARAMETER_NODE_SETTER_CREATE_H

#include "pt/parameter_node_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
    class constant;
}}

namespace pt { namespace d3d9 { namespace rendering {

class constant_setter;

std::unique_ptr<constant_setter> create_parameter_node_setter(
    std::shared_ptr<parameter_node> node, constant const& c);

}}} // namespace pt::d3d9::rendering

#endif
