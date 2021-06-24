#include "pixel_shader.h"
#include "constant_buffer.h"
#include "device_convenience.h"

namespace pt { namespace d3d9 {

pixel_shader::pixel_shader(graphics_device& device,
    com_ptr<IDirect3DPixelShader9> native,
    com_ptr<ID3DXConstantTable> constants)
: shader(device)
, m_native(native)
, m_constants(new constant_buffer(device, constants))
{
}

pixel_shader::pixel_shader(pixel_shader const& other)
: shader(other)
, m_native(other.m_native)
, m_constants(new constant_buffer(*other.m_constants))
{
}

pixel_shader::~pixel_shader()
{
}

void pixel_shader::use() const
{
    set_pixel_shader(mutable_device(), m_native);
}

constant_buffer const& pixel_shader::constants() const
{
    return *m_constants;
}

constant_buffer& pixel_shader::constants()
{
    return *m_constants;
}

void pixel_shader::before_destruction()
{
    m_native.reset();
}

void pixel_shader::before_reset()
{
    assert(!"todo");
}

void pixel_shader::after_reset()
{
    assert(!"todo");
}

}} // namespace pt::d3d9
