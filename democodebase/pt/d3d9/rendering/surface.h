#ifndef PT_D3D9_RENDERING_SURFACE_H
#define PT_D3D9_RENDERING_SURFACE_H

#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt {
    class parameter_collection;
}

namespace pt { namespace d3d9 { namespace shaders {
    class surface_shader;
}}}

namespace pt { namespace d3d9 { namespace rendering {

class surface_variables;
class variable_owner;

class surface_variables_instance
{
public:
    surface_variables_instance(
        std::shared_ptr<surface_variables> variables,
        std::shared_ptr<surface_variables> pre_shader_variables);

    variable_owner const& variables() const;
    variable_owner& variables();

    variable_owner const& pre_shader_variables() const;
    variable_owner& pre_shader_variables();

    std::shared_ptr<shaders::surface_shader const> shader() const;
    std::shared_ptr<shaders::surface_shader const> pre_shader() const;

    parameter_collection const& parameters() const;

private: // nonassignable
    surface_variables_instance const& operator=(surface_variables_instance const&);

private: // data members
    std::shared_ptr<surface_variables> m_variables;
    std::shared_ptr<surface_variables> m_pre_shader_variables;
};

/// Surface. Synonym to surface material.
class surface
{
public:
    surface(std::vector<surface_variables_instance> instances);

    std::vector<surface_variables_instance> const& instances() const;
    std::vector<surface_variables_instance>& instances();

private: // nonassignable
    surface const& operator=(surface const&);
    
private: // data members
    std::vector<surface_variables_instance> m_instances;
};

}}} // namespace pt::d3d9::rendering

#endif
