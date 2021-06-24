#include "rotation.h"
#include "../utility.h"
#include "pt/matrix_operations.h"
#include "pt/constant_value_source.h"
#include "pt/math/quaternion.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    inline D3DXVECTOR3 get_vector(math::quaternionf const& q)
    {
        return D3DXVECTOR3(q.R_component_2(), q.R_component_3(),
            q.R_component_4());
    }

    std::shared_ptr<constant_value_source<math::quaternionf> > create_source()
    {
        math::quaternionf q(1, 0, 0, 0);
        std::shared_ptr<constant_value_source<math::quaternionf> > source(
            new constant_value_source<math::quaternionf>(q));

        return source;
    }

    matrix4x4f calculate_local_to_parent_at(float time, animatable_value<math::quaternionf> const& quaternion)
    {
        math::quaternionf const value = quaternion.value_at(time);

        D3DXQUATERNION q;
        q.w = value.R_component_1();
        q.x = value.R_component_2();
        q.y = value.R_component_3();
        q.z = value.R_component_4();


        D3DXQuaternionNormalize(&q, &q);

        D3DXMATRIX mat;
        D3DXMatrixRotationQuaternion(&mat, &q);

        return convert(mat);
    }
}

rotation::rotation()
: m_quaternion(create_source())
, m_time(0.0f)
{
    m_local_to_parent = calculate_local_to_parent_at(m_time, m_quaternion);
}

rotation::rotation(animatable_value<math::quaternionf> const& quaternion)
: m_quaternion(quaternion)
, m_time(0.0f)
{
    if (m_quaternion.is_constant())
        m_local_to_parent = calculate_local_to_parent_at(m_time, m_quaternion);
}

matrix4x4f rotation::get_local_to_parent_at(float time) const
{
    if (m_quaternion.is_constant())
        return m_local_to_parent;

    if (time != m_time)
    {
        m_local_to_parent = calculate_local_to_parent_at(time, m_quaternion);
    }

    return m_local_to_parent;

/*    D3DXMATRIX mat;
    D3DXQUATERNION q;
    math::quaternionf value = m_quaternion.value_at(time);

    float angle = -value.R_component_1();
    D3DXVECTOR3 axis = get_vector(value[1], value[2], value[3]);

    float angle = -value[0];

    D3DXVECTOR3 axis(value[1], value[2], value[3]);
    D3DXQuaternionRotationAxis(&q, &axis, angle);

    D3DXMatrixRotationQuaternion(&mat, &q);

    return convert(mat);*/
}

bool rotation::get_is_constant() const
{
    return m_quaternion.is_constant();
}

namespace {
    //template<typename T>
    //class quaternion_value_source :
    //    public animatable_value_source<math::quaternionf>
    //{
    //public:
    //    quaternion_value_source(T value);

    //public: // interface required by animatable_value_source<math::quaternionf>
    //    bool is_constant() const;
    //    math::quaternionf value_at(float) const;

    //private:
    //    math::quaternionf m_value;
    //    math::quaternionf
    //};

    //typedef math::vector4f_data_set<math::kochanek_bartels_data_set<float> >
    //    kb_quaternion_data_set;

    ///// quaternion floating point Kochanek-Bartels hermite spline.
    //typedef cubic_hermite_spline4<kochanek_bartels_data_set<float> >
    //    kb_quaternion;
}

//animatable_value<math::quaternionf> quaternion_value_from_axis_angles(
//    math::data_set<float> [4])
//{
//    assert(false);
//    return animatable_value<math::quaternionf>();
//}

}}} // namespace pt::d3d9::rendering
