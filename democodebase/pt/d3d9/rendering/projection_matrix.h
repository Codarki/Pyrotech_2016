#ifndef PT_D3D9_RENDERING_PROJECTION_MATRIX_H
#define PT_D3D9_RENDERING_PROJECTION_MATRIX_H

#include "pt/matrix.h"
#include "pt/std/memory.h"

namespace pt {
    template<typename T>
    class animatable_value;
}

namespace pt { namespace d3d9 { namespace rendering {

class composite_variable_owner;

/// Shares calculation of projection matrix
class projection_matrix
{
public:
    projection_matrix(std::shared_ptr<animatable_value<float> const> fov);
    void set_resolution(vector2i resolution);
    void set_zplanes(float near, float far);
    void update(float time);
    matrix4x4f const& get() const;

private: // data members
    matrix4x4f m_projection;
    vector2i m_resolution;
    std::shared_ptr<animatable_value<float> const> m_fov;
    float m_time;

    float m_near_plane;
    float m_far_plane;
};

}}} // namespace pt::d3d9::rendering

#endif
