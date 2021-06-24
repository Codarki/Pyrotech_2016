#ifndef PT_INTERSECTION_H
#define PT_INTERSECTION_H

#include "box.h"
#include <boost/optional.hpp>

namespace pt {

template<typename T>
inline optional<vector<T,2> > line_line_intersection(
    vector<T,2> const& a_start, vector<T,2> const& a_end,
    vector<T,2> const& b_start, vector<T,2> const& b_end, bool within_lines)
{
    // point-slope form of a line: m * (x - x0) = y - y0, where m is the slope
    // (y1 - y0) / (x1 - x0)

    // rearrange the equation to: m*x + (-1)*y = m*x0 - y0
    // which is of the form: A*x + B*y = C

    // slopes
    vector2f edge_a = static_vector_cast<float>(a_end - a_start);
    vector2f edge_b = static_vector_cast<float>(b_end - b_start);

    // beware of infinity in the slopes..
    float m_a = fabs(edge_a[0]) > static_cast<T>(0.0001)
        ? edge_a[1] / edge_a[0] : static_cast<float>(1e+4);
    float m_b = fabs(edge_b[0]) > static_cast<T>(0.0001)
        ? edge_b[1] / edge_b[0] : static_cast<float>(1e+4);

    float a1 = m_a;
    float a2 = m_b;
    float b1 = static_cast<float>(-1);
    float b2 = static_cast<float>(-1);
    float c1 = m_a * a_start[0] - a_start[1];
    float c2 = m_b * b_start[0] - b_start[1];

    // Use Cramers rule. 
    float inv_det = 1.0f / (a1*b2 - a2*b1);
    float det_x = (c1*b2 - c2*b1);
    float det_y = (a1*c2 - a2*c1);

    vector2f intersection_float(det_x * inv_det, det_y * inv_det);
    vector<T,2> intersection = static_vector_cast<T>(intersection_float);

    if (within_lines)
    {
        // If the intersection is further from the center of the line, than
        // start or end position, no hit.
        vector2f a_start_float = static_vector_cast<float>(a_start);
        vector2f a_end_float = static_vector_cast<float>(a_end);
        vector2f b_start_float = static_vector_cast<float>(b_start);
        vector2f b_end_float = static_vector_cast<float>(b_end);

        vector2f center_a = (a_start_float + a_end_float) * 0.5f;
        vector2f center_b = (b_start_float + b_end_float) * 0.5f;

        float squared_half_length_a = squared_distance(a_start_float, center_a);
        float squared_half_length_b = squared_distance(b_start_float, center_b);
        float squared_length_in_a = squared_distance(intersection_float, center_a);
        float squared_length_in_b = squared_distance(intersection_float, center_b);

        if (squared_length_in_a < squared_half_length_a
            && squared_length_in_b < squared_half_length_b)
        {
            return optional<vector<T,2> >(intersection);
        }
    }
    return optional<vector<T,2> >();
}

template<typename T>
inline optional<vector<T,2> > first_line_box_intersection(box<T,2> const& box,
    vector<T,2> const& a_start, vector<T,2> const& a_end, bool within_lines)
{
    vector<T,2> half_axis = box.half_axis_lengths();
    vector<T,2> corners[4];
    corners[0] = box.center() + vector<T,2>(-half_axis[0], -half_axis[1]);
    corners[1] = box.center() + vector<T,2>(half_axis[0], -half_axis[1]);
    corners[2] = box.center() + vector<T,2>(half_axis[0], half_axis[1]);
    corners[3] = box.center() + vector<T,2>(-half_axis[0], half_axis[1]);

    for (size_t i = 0; i < 4; ++i)
    {
        size_t index_0 = i;
        size_t index_1 = (i + 1) != 4 ? i + 1 : 0;

        optional<vector<T,2> > intersection = line_line_intersection(a_start,
            a_end, corners[index_0], corners[index_1], within_lines);

        if (intersection)
            return intersection;
    }
    return optional<vector<T,2> >();
}

} // namespace pt

#endif
