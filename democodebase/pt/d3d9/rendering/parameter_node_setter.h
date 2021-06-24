#pragma once
#ifndef PT_D3D9_RENDERING_PARAMETER_NODE_SETTER_H
#define PT_D3D9_RENDERING_PARAMETER_NODE_SETTER_H

#include "constant_setter.h"
#include "pt/d3d9/constant_utility.h"
#include "pt/parameter_node_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

template<typename T>
class parameter_node_setter : public constant_setter
{
public:
    parameter_node_setter(std::shared_ptr<parameter_node> node);

public: // interface required by variable_setter
    void update_target(float time) const;

private: // data members
    std::shared_ptr<parameter_node> m_node;
};

// .inl

template<typename T>
parameter_node_setter<T>::parameter_node_setter(
    std::shared_ptr<parameter_node> node)
:   m_node(node)
{
}

template<typename T>
void parameter_node_setter<T>::update_target(float time) const
{
    set_constant(constant(), constants(), value_at<T>(time, *m_node));
}

}}} // namespace pt::d3d9::rendering

#endif
