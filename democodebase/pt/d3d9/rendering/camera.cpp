#include "camera.h"

namespace pt { namespace d3d9 { namespace rendering {

// camera

camera::camera(composite_variable_owner variables,
    std::shared_ptr<projection_matrix> projection,
    std::vector<animatable_value<math::plane>> clipping_planes,
    bool flip_culling_mode)
: m_variables(variables)
, m_projection(projection)
, m_clipping_planes(clipping_planes)
, m_flip_culling_mode(flip_culling_mode)
{
}

projection_matrix& camera::projection()
{
    return *m_projection;
}

variable_owner& camera::variables()
{
    return m_variables;
}

variable_owner const& camera::variables() const
{
    return m_variables;
}

std::shared_ptr<projection_matrix> camera::shared_projection() const
{
    return m_projection;
}

std::vector<animatable_value<math::plane>> const& camera::clipping_planes() const
{
    return m_clipping_planes;
}

bool camera::flip_culling() const
{
    return m_flip_culling_mode;
}

}}} // namespace pt::d3d9::rendering
