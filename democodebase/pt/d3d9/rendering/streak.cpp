#include "streak.h"
#include "post_processing.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/format.h"
#include "pt/exceptions/string_context.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    texture_2d create_result_texture(graphics_device& device,
        vector2i resolution, D3DFORMAT format)
    {
        return texture_2d_builder(device)
            .resolution(resolution)
            .levels(1)
            .usage(D3DUSAGE_RENDERTARGET)
            .format(format)
            .pool(D3DPOOL_DEFAULT)
            .build();
    }
} // anonymous namespace

streak::streak(graphics_device& device, vector2i resolution, D3DFORMAT format)
:   m_texture(create_result_texture(device, resolution, format))
{
}

void streak::draw(texture_2d const& source)
{
    graphics_device& device = source.mutable_device();
    set_texture(source, 0);

    scoped_render_target_texture rt(m_texture);
    fill_fullscreen_quad(device);
    set_texture(device, 0, com_ptr<IDirect3DTexture9>());
}

texture_2d const& streak::result() const
{
    return m_texture;
}

vector2i streak::resolution() const
{
    return m_resolution;
}

}}} // namespace pt::d3d9::rendering
