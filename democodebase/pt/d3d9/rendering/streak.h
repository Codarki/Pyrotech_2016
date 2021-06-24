#ifndef PT_D3D9_RENDERING_STREAK_H
#define PT_D3D9_RENDERING_STREAK_H

#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/vector.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {
    class pixel_shader;
    class vertex_shader;
}}

namespace pt { namespace d3d9 { namespace rendering {

/// Streaks.
class streak
{
public:
    streak(graphics_device& device, vector2i resolution, D3DFORMAT format);

    void draw(texture_2d const& source);

    texture_2d const& result() const;
    vector2i resolution() const;

private: // data members
	std::shared_ptr<pixel_shader> m_pixel_shader;
	std::shared_ptr<vertex_shader> m_vertex_shader;
    texture_2d m_texture;
    std::vector<vector4f> m_samples;
    vector2i m_resolution;
};

}}} // namespace pt::d3d9::rendering

#endif
