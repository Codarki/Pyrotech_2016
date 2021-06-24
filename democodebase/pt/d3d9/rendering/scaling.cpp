#include "scaling.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 { namespace rendering {

scaling::scaling(animatable_value<vector3f> const& scale)
:   m_scale(scale)
{
}

matrix4x4f scaling::get_local_to_parent_at(float time) const
{
    return scaling_matrix(m_scale.value_at(time));
}

bool scaling::get_is_constant() const
{
    return m_scale.is_constant();
}

}}} // namespace pt::d3d9::rendering
