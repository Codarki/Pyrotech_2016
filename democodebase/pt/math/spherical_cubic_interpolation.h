#ifndef PT_MATH_SPHERICAL_CUBIC_INTERPOLATION_H
#define PT_MATH_SPHERICAL_CUBIC_INTERPOLATION_H

#include "data_set.h"
#include "quaternion.h"
#include "pt/boost/assert.h"

namespace pt { namespace math {

class spherical_cubic_interpolation
{
public:
    typedef boost::math::quaternion<float> result_type;
    typedef kb_data_set<boost::math::quaternion<float> > dataset;

public:
    spherical_cubic_interpolation(dataset const& data);
    result_type value_at(float time) const;

private:
    dataset m_data;
};

typedef spline_value_source<spherical_cubic_interpolation>
    squad_value_source;

// .inl

inline spherical_cubic_interpolation::spherical_cubic_interpolation(
    dataset const& data)
:   m_data(data)
{
    assert(m_data.size() > 0);
}

inline D3DXQUATERNION normalize_d3dx(D3DXQUATERNION const& x)
{
    D3DXQUATERNION result;
    D3DXQuaternionNormalize(&result, &x);
    return result;
}

inline D3DXQUATERNION quaternion_inner_point(
    D3DXQUATERNION const& Q_prev,
    D3DXQUATERNION const& Q,
    D3DXQUATERNION const& Q_next)
{
    assert(is_unit_length(Q_prev));
    assert(is_unit_length(Q));
    assert(is_unit_length(Q_next));

    D3DXQUATERNION inv_Q;
    inv_Q = reciprocal(Q);

    assert(is_unit_length(inv_Q));

    D3DXQUATERNION log_1 = inv_Q * Q_prev;
    D3DXQuaternionLn(&log_1, &log_1);

    D3DXQUATERNION log_2 = inv_Q * Q_next;
    D3DXQuaternionLn(&log_2, &log_2);

    D3DXQUATERNION tmp = -(log_1 + log_2) / 4.0f;
    D3DXQuaternionExp(&tmp, &tmp);

    D3DXQUATERNION result = Q * tmp;

    return result;
}

/*
g1 = slerp(q1, q0, -(1 + B) / 3.0)
g2 = slerp(q1, q2, (1 - B) / 3.0)

control point case a:
g3 = slerp(g1, g2, 0.5 + 0.5 * C)
an = slerp(q1, g3, (T - 1))

control point case b:
g3 = slerp(g1, g2, 0.5 - 0.5 * C)
bn = slerp(q1, g3, -(T - 1))

incoming spline case a is valid, for the outgoing case b.
*/

// by Jochen Wilhelmy
inline void quaternion_tangent_digisnap(
    D3DXQUATERNION const& Q_prev,
    D3DXQUATERNION const& Q,
    D3DXQUATERNION const& Q_next,
    float tension, float continuity, float bias,
    D3DXQUATERNION& incoming,
    D3DXQUATERNION& outgoing)
{
    D3DXQUATERNION g1 = slerp(Q, Q_prev, -(1.0f + bias) / 3.0f);
    D3DXQUATERNION g2 = slerp(Q, Q_next, (1.0f - bias) / 3.0f);

    assert(is_unit_length(g1));
    assert(is_unit_length(g2));

    D3DXQUATERNION g3 = slerp(g1, g2, 0.5f + 0.5f * continuity);
    incoming = slerp(Q, g3, tension - 1.0f);

    assert(is_unit_length(g3));
    assert(is_unit_length(incoming));

    D3DXQUATERNION g4 = slerp(g1, g2, 0.5f - 0.5f * continuity);
    outgoing = slerp(Q, g4, -(tension - 1.0f));

    assert(is_unit_length(g4));
    assert(is_unit_length(outgoing));

    //tension;
    //incoming = g3;
    //outgoing = g4;
}

// D3DXQuaternionSquad is:
// Slerp(Slerp(pQ1, pC, t), Slerp(pA, pB, t), 2t(1 - t))
//
// The slerp function assumes that the two input quaternions form an acute
// angle (their dot product is nonnegative).
//
inline D3DXQUATERNION quaternion_squad(
    D3DXQUATERNION const& Q1,
    D3DXQUATERNION A,
    D3DXQUATERNION const& B,
    D3DXQUATERNION Q2,
    float time)
{
    assert(is_unit_length(Q1));
    assert(is_unit_length(A));
    assert(is_unit_length(B));
    assert(is_unit_length(Q2));

    D3DXQUATERNION tmp1 = slerp(Q1, Q2, time);
    D3DXQUATERNION tmp2 = slerp(A, B, time);

    assert(is_unit_length(tmp1));
    assert(is_unit_length(tmp2));

    // tmp1 is direct slerp from key1 to key2.

    return normalize(slerp(tmp1, tmp2, 2.0f * time * (1.0f - time)));
}


D3DXQUATERNION to_d3dx_quaternion(boost::math::quaternion<float> const& q)
{
    return D3DXQUATERNION(
        q.R_component_2(),
        q.R_component_3(),
        q.R_component_4(),
        q.R_component_1());
}

inline boost::math::quaternion<float>
    spherical_cubic_interpolation::value_at(float time) const
{
    dataset::const_iterator iter = m_data.optional_endpoint(time);
    if (iter != m_data.end())
    {
        return iter->value();
    }

    assert(m_data.size() >= 2);

    dataset::point_pair points = m_data.points_at(time);

    if (points.second == m_data.end())
    {
        // exact match.
        return points.first->value();
    }

    dataset::const_iterator q0_iter = points.first;
    dataset::const_iterator q1_iter = points.first;
    dataset::const_iterator q2_iter = points.second;
    dataset::const_iterator q3_iter = points.second + 1;
    
    assert(q1_iter != m_data.end());
    assert(q2_iter != m_data.end());

    float time1 = q1_iter->time();
    float time2 = q2_iter->time();
    assert(time2 > time1);

    float t_range = time2 - time1;
    float t_01 = (time - time1) / t_range;

    assert(t_01 >= 0.0f && t_01 <= 1.0f);

    if (t_range < 2)
    {
        // Linear interpolate key intervals less than 2 frames.
        // SLERP
       D3DXQUATERNION Q0(
            q1_iter->value().R_component_2(),
            q1_iter->value().R_component_3(),
            q1_iter->value().R_component_4(),
            q1_iter->value().R_component_1());
        D3DXQUATERNION Q1(
            q2_iter->value().R_component_2(),
            q2_iter->value().R_component_3(),
            q2_iter->value().R_component_4(),
            q2_iter->value().R_component_1());

        float t1 = q1_iter->time();
        float t2 = q2_iter->time();
        assert(t2 > t1);

        float t_range2 = t2 - t1;
        float t_01_2 = (time - t1) / t_range2;

        assert(t_01_2 >= 0.0f && t_01_2 <= 1.0f);
        D3DXQUATERNION out;
        
        D3DXQuaternionSlerp(&out, &Q0, &Q1, t_01_2);

        out = normalize(out);

        return math::quaternionf(out.w, out.x, out.y, out.z);
    }

    bool is_first = false;
    if (q0_iter != m_data.begin())
    {
        --q0_iter;
    }
    else
    {
        is_first = true;
    }

    bool is_last = false;
    if (q3_iter == m_data.end())
    {
        q3_iter = q2_iter;
        is_last = true;
    }

    D3DXQUATERNION Q0(
        q0_iter->value().R_component_2(),
        q0_iter->value().R_component_3(),
        q0_iter->value().R_component_4(),
        q0_iter->value().R_component_1());
    D3DXQUATERNION Q1(
        q1_iter->value().R_component_2(),
        q1_iter->value().R_component_3(),
        q1_iter->value().R_component_4(),
        q1_iter->value().R_component_1());
    D3DXQUATERNION Q2(
        q2_iter->value().R_component_2(),
        q2_iter->value().R_component_3(),
        q2_iter->value().R_component_4(),
        q2_iter->value().R_component_1());
    D3DXQUATERNION Q3(
        q3_iter->value().R_component_2(),
        q3_iter->value().R_component_3(),
        q3_iter->value().R_component_4(),
        q3_iter->value().R_component_1());

    Q1 = normalize(Q1);
    Q2 = normalize(Q2);

    if (is_first)
    {
        Q0 = (2 * Q1) - Q2;
    }
    if (is_last)
    {
        Q3 = (2 * Q2) - Q1;
    }

    Q0 = normalize(Q0);
    Q3 = normalize(Q3);

    D3DXQUATERNION A = quaternion_inner_point(Q0, Q1, Q2);
    D3DXQUATERNION B = quaternion_inner_point(Q1, Q2, Q3);

    //    // Speed adjustment.
    //    float t_prev = q1_iter->time() - q0_iter->time();
    //    float t_cur = q2_iter->time() - q1_iter->time();
    //    float t_total = t_prev + t_cur;
    //    float starting_coeff = (2.0f * t_cur) / t_total;
    //    float ending_coeff = (2.0f * t_prev) / t_total;

    //if (starting_coeff > 0.0001f)
    //{
    //    A *= starting_coeff;
    //    A = normalize(A);
    //}
    //if (ending_coeff > 0.0001f)
    //{
    //    B *= ending_coeff;
    //    B = normalize(B);
    //}

    D3DXQUATERNION out = quaternion_squad(Q1, A, B, Q2, t_01);
    return math::quaternionf(out.w, out.x, out.y, out.z);

    /*
    D3DXQUATERNION A_incoming;
    D3DXQUATERNION A_outgoing;
    D3DXQUATERNION B_incoming;
    D3DXQUATERNION B_outgoing;

    quaternion_tangent_digisnap(Q0, Q1, Q2,
        q1_iter->parameters().tension,
        q1_iter->parameters().continuity,
        q1_iter->parameters().bias,
        A_incoming, A_outgoing);

    quaternion_tangent_digisnap(Q1, Q2, Q3,
        q2_iter->parameters().tension,
        q2_iter->parameters().continuity,
        q2_iter->parameters().bias,
        B_incoming, B_outgoing);

    //D3DXQUATERNION A = A_outgoing;
    //D3DXQUATERNION B = B_incoming;

    D3DXQUATERNION p0 = to_d3dx_quaternion(points.first->value());
    D3DXQUATERNION p1 = to_d3dx_quaternion(points.second->value());
    //D3DXQUATERNION t0 = points.first->starting_tangent();
    //D3DXQUATERNION t1 = points.second->ending_tangent();
    D3DXQUATERNION t0 = A_incoming;
    D3DXQUATERNION t1 = B_outgoing;

    float h = points.second->time() - points.first->time();
    float t = (time - points.first->time()) / h;
    float t2 = t * t;
    float t3 = t * t * t;

    float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    float h10 = t3 - 2.0f * t2 + t;
    float h01 = -2.0f * t3 + 3.0f * t2;
    float h11 = t3 - t2;

    D3DXQUATERNION out = h00 * p0 + h10 * t0 + h01 * p1 + h11 * t1;

    out = normalize(out);

    return math::quaternionf(out.w, out.x, out.y, out.z);
    */
}

}} // namespace pt::math

#endif
