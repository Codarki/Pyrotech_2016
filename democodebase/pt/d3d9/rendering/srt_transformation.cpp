#include "srt_transformation.h"
#include "pt/matrix_operations.h"
#include "pt/d3d9/utility.h"

namespace pt { namespace d3d9 { namespace rendering {

// srt_transformation

srt_transformation::srt_transformation(scaling scale, rotation rotation,
    translation translation)
: m_scale(scale)
, m_rotation(rotation)
, m_translation(translation)
, m_time(0.0f)
{
    m_local_to_parent = impl_local_to_parent_at(m_time);
}

matrix4x4f srt_transformation::get_local_to_parent_at(float time) const
{
    // Use cached value if S, R and T are constant.
    if (impl_is_constant())
        m_local_to_parent;

    // Use cached value if time matches.
    if (time != m_time)
    {
        m_local_to_parent = impl_local_to_parent_at(time);
        m_time = time;
    }

    return m_local_to_parent;
}

bool srt_transformation::get_is_constant() const
{
    return impl_is_constant();
}

/// Nonvirtual helper function
matrix4x4f srt_transformation::impl_local_to_parent_at(float time) const
{
    return m_scale.local_to_parent_at(time)
        * m_rotation.local_to_parent_at(time)
        * m_translation.local_to_parent_at(time);
}

/// Nonvirtual helper function
bool srt_transformation::impl_is_constant() const
{
    return m_translation.is_constant() && m_rotation.is_constant() && m_scale.is_constant();
}

// pivoted_srt_transformation

pivoted_srt_transformation::pivoted_srt_transformation(scaling scale, rotation rotation,
    translation translation, vector3f pivot)
    : m_scale(scale)
    , m_rotation(rotation)
    , m_translation(translation)
    , m_pivot(pivot)
{
    m_pivot_translation = translation_matrix(-m_pivot);
}

matrix4x4f pivoted_srt_transformation::get_local_to_parent_at(float time) const
{
    matrix4x4f result = m_scale.local_to_parent_at(time)
        * m_pivot_translation
        * m_rotation.local_to_parent_at(time)
        * m_translation.local_to_parent_at(time);

    return result;
}

bool pivoted_srt_transformation::get_is_constant() const
{
    return m_translation.is_constant() && m_rotation.is_constant() && m_scale.is_constant();
}

}}} // namespace pt::d3d9::rendering
