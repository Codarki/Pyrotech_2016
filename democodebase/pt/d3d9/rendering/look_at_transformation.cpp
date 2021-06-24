#include "look_at_transformation.h"
#include "pt/d3d9/utility.h"
#include "pt/animatable_value_utility.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 { namespace rendering {

look_at_transformation::look_at_transformation(
    std::shared_ptr<transformation> position,
    std::shared_ptr<transformation> target)
:   m_position(position)
,   m_target(target)
,   m_roll(create_constant_value(1.0f))
,   m_current_time(-1000.0f)
{
}

look_at_transformation::look_at_transformation(
    std::shared_ptr<transformation> position,
    std::shared_ptr<transformation> target,
    std::shared_ptr<animatable_value<float> > roll)
:   m_position(position)
,   m_target(target)
,   m_roll(roll)
,   m_current_time(-1000.0f)
{
}

matrix4x4f look_at_transformation::get_local_to_parent_at(float time) const
{
    //time = time * 30.0f;

    if (time != m_current_time)
    {
        m_parent_to_local = parent_to_local_at(time);
        m_local_to_parent = invert(m_parent_to_local);
        m_current_time = time;
    }
    return m_local_to_parent;
}

bool look_at_transformation::get_is_constant() const
{
    return m_position->is_constant() && m_target->is_constant() && m_roll->is_constant();
}

matrix4x4f look_at_transformation::parent_to_local_at(float time) const
{
    vector3f position = translation(m_position->local_to_parent_at(time));
    vector3f target = translation(m_target->local_to_parent_at(time));

    float roll = m_roll->value_at(time);
    vector3f up(0, roll, 0);

    D3DXVECTOR3 d3dx_eye(&position[0]);
    D3DXVECTOR3 d3dx_at(&target[0]);
    D3DXVECTOR3 d3dx_up(&up[0]);

    D3DXMATRIX result;
    D3DXMatrixLookAtLH(&result, &d3dx_eye, &d3dx_at, &d3dx_up);

    return convert(result);
}

}}} // namespace pt::d3d9::rendering
