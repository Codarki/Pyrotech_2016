#ifndef PT_D3D9_RENDERING_BLUR_H
#define PT_D3D9_RENDERING_BLUR_H

#include "pt/d3d9/texture_2d.h"
//#include "pt/d3d9/texture_2d_source_base.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/animatable_value.h"
#include "pt/parameter_collection.h"
#include "pt/vector.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt {
    class parameter_collection;
}

namespace pt { namespace d3d9 {
    class pixel_shader;
    class vertex_shader;
}}

namespace pt { namespace d3d9 { namespace rendering {

// Perform a 5x5 gaussian blur on source texture.
class blur
{
public:
    blur(
        graphics_device& device,
        vector2i resolution,
        D3DFORMAT format,
        std::shared_ptr<animatable_value<std::shared_ptr<texture_2d>> const> blur_strength_texture);

    void update(float time);

    void draw(texture_2d const& source);

    texture_2d const& result() const;
    vector2i resolution() const;

private: // data members
    std::shared_ptr<pixel_shader> m_pixel_shader;
    std::shared_ptr<vertex_shader> m_vertex_shader;
    std::shared_ptr<animatable_value<std::shared_ptr<texture_2d>> const> m_blur_strength_texture;
    texture_2d m_texture;

    std::vector<vector4f> m_offsets;
    std::vector<vector4f> m_weights;
    parameter_collection m_shared_parameters;

    float m_current_time;
};

}}} // namespace pt::d3d9::rendering

#endif
