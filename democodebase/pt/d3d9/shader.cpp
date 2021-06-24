#include "shader.h"
#include "shader_builder.h"
#include "device_convenience.h"

namespace pt { namespace d3d9 {

// shader

shader::shader(graphics_device& device)
: device_child(device)
{
}

shader::~shader()
{
}

// shader_builder

shader_builder::shader_builder(graphics_device& device)
: m_device(&device)
{
}

shader_builder::~shader_builder()
{
}

com_ptr<IDirect3DPixelShader9> shader_builder::build_pixel_shader(
    DWORD const* function)
{
    return create_pixel_shader(*m_device, function);
}

com_ptr<IDirect3DVertexShader9> shader_builder::build_vertex_shader(
    DWORD const* function)
{
    return create_vertex_shader(*m_device, function);
}

}} // namespace pt::d3d9
