#ifndef CONTENT_PLASMA_H
#define CONTENT_PLASMA_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/d3d9/texture_2d_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
    class vertex_buffer;
    class pixel_shader;
    class vertex_shader;
}}

namespace content {

using namespace pt;

class plasma
{
public:
    plasma(d3d9::graphics_device& device);

    void draw() const;
    void update(float time);

private: // data members
    float m_time;
    std::shared_ptr<d3d9::texture_2d> m_background_texture;
    std::shared_ptr<d3d9::vertex_buffer> m_vertex_buffer;
    std::shared_ptr<d3d9::pixel_shader> m_pixel_shader;
    std::shared_ptr<d3d9::vertex_shader> m_vertex_shader;
    mutable std::unique_ptr<d3d9::texture_2d> m_texture;

};

} // namespace content

#endif
