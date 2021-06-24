#include "translation.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 { namespace rendering {

translation::translation(animatable_value<vector3f> value)
:   m_translation(value)
{
}

matrix4x4f translation::get_local_to_parent_at(float time) const
{
    return translation_matrix(m_translation.value_at(time));
}

bool translation::get_is_constant() const
{
    return m_translation.is_constant();
}

}}} // namespace pt::d3d9::rendering
