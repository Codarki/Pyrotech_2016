#pragma once
#ifndef PT_D3D9_RENDERING_LIGHT_PROPERTIES_H
#define PT_D3D9_RENDERING_LIGHT_PROPERTIES_H

#include "pt/std/memory.h"

namespace pt {
    class parameter_collection;
}

namespace pt { namespace d3d9 { namespace shaders {
    class light_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering {

class variable_owner;
class light_variables2;

/// Light properties. "Material" for lights. Same as surface is for a geometry.
class light_properties
{
public:
    explicit light_properties(
        std::shared_ptr<light_variables2> variables);

    variable_owner const& variables() const;
    variable_owner& variables();

    std::shared_ptr<shaders::light_shader const> shader() const;

    parameter_collection const& parameters() const;

private: // data members    
    std::shared_ptr<light_variables2> m_variables;
};

}}} // namespace pt::d3d9::rendering

#endif
