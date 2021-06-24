#include "projection_matrix.h"
#include "pt/d3d9/utility.h"
#include "pt/animatable_value_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

projection_matrix::projection_matrix(
    std::shared_ptr<animatable_value<float> const> fov)
: m_fov(fov)
, m_resolution(1024, 768)
, m_time(-666.0f)
, m_near_plane(1.0f)
, m_far_plane(1000.0f)
{
}
void projection_matrix::set_resolution(vector2i resolution)
{
    m_resolution = resolution;

    // Use time as internal invalidate flag for now.
    m_time = -666.0f;
}
void projection_matrix::set_zplanes(float near_plane, float far_plane)
{
    m_near_plane = near_plane;
    m_far_plane = far_plane;
    // Use time as internal invalidate flag for now.
    m_time = -666.0f;
}
void projection_matrix::update(float time)
{
    if (time == m_time)
        return;

    m_time = time;

    float fov_y_degrees = m_fov->value_at(time);
    if (fov_y_degrees < 0.0f)
        fov_y_degrees = 0.1f;
    assert(fov_y_degrees > 0.0f);

    float fov_y = fov_y_degrees / 360.0f * (2.0f * 3.1415f);
    float aspect = m_resolution[0] / static_cast<float>(m_resolution[1]);
    float zn = m_near_plane;
    float zf = m_far_plane;
        
    D3DXMATRIX mat;
    D3DXMatrixPerspectiveFovLH(&mat, fov_y, aspect, zn, zf);
        
    m_projection = d3d9::convert(mat);
}
matrix4x4f const& projection_matrix::get() const
{
    return m_projection;
}

}}} // namespace pt::d3d9::rendering
