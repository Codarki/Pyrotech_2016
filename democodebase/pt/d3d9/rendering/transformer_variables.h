#ifndef PT_D3D9_RENDERING_TRANSFORMER_VARIABLES_H
#define PT_D3D9_RENDERING_TRANSFORMER_VARIABLES_H

#include "simple_variable_owner.h"
#include "pt/basic_value_source.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 { namespace rendering {

/// Handles creation of transformation variable setters for shaders.
class transformer_variables : public simple_variable_owner
{
public:
    transformer_variables();
    transformer_variables(matrix4x4f const& local_to_world,
        matrix4x4f const& world_to_local);
    ~transformer_variables();

    void set_local_to_world(matrix4x4f const& matrix);
    void set_world_to_local(matrix4x4f const& matrix);

    /// Name of the matrix in hlsl. eg. "object_to_world" or "light_to_world"
    void set_local_to_world_name(std::string const& name);
    void set_world_to_local_name(std::string const& name);

protected: // interface required by simple_variable_owner
    std::unique_ptr<constant_setter> on_create_constant(
        constant const& constant) const;

private: // noncopyable
    transformer_variables(transformer_variables const&);
    transformer_variables const& operator=(transformer_variables const&);

private: // data members
    std::string m_local_to_world_name;
    std::string m_world_to_local_name;

    basic_value_source<matrix4x4f> m_local_to_world;
    basic_value_source<matrix4x4f> m_world_to_local;
};

}}} // namespace pt::d3d9::rendering

#endif
