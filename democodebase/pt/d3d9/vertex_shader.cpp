#include "vertex_shader.h"
#include "constant_buffer.h"
#include "device_convenience.h"

namespace pt { namespace d3d9 {

vertex_shader::vertex_shader(graphics_device& device, com_ptr<IDirect3DVertexShader9> native,
    com_ptr<ID3DXConstantTable> constants)
: shader(device)
, m_native(native)
, m_constants(new constant_buffer(device, constants))
{
}

vertex_shader::vertex_shader(vertex_shader const& other)
: shader(other)
, m_native(other.m_native)
, m_constants(new constant_buffer(*other.m_constants))
{
}

vertex_shader::~vertex_shader()
{
}


void vertex_shader::use() const
{
    set_vertex_shader(mutable_device(), m_native);
}

constant_buffer const& vertex_shader::constants() const
{
    return *m_constants;
}

constant_buffer& vertex_shader::constants()
{
    return *m_constants;
}

void vertex_shader::before_destruction()
{
    m_native.reset();
}

void vertex_shader::before_reset()
{
    assert(!"todo");
}

void vertex_shader::after_reset()
{
    assert(!"todo");
}

}} // namespace pt::d3d9
