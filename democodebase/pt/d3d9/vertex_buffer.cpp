#include "vertex_buffer.h"
#include "graphics_device.h"
#include "error.h"
#include "pt/none.h"
#include "pt/any_error.h"
#include "pt/boost/assert.h"

namespace pt { namespace d3d9 {

vertex_buffer::vertex_buffer(graphics_device& device, DWORD bytes,
    DWORD usage, DWORD fvf, D3DPOOL pool)
:   resource(device)
,   m_bytes(bytes)
,   m_usage(usage)
,   m_fvf(fvf)
,   m_pool(pool)
{
    // TODO move to higher level. Include item name in error.
    if (bytes == 0)
        throw pt::any_error("Zero bytes in vertex_buffer");

    create_native();
}

vertex_buffer::~vertex_buffer()
{
}

com_ptr<IDirect3DVertexBuffer9>& vertex_buffer::native()
{
    return m_native;
}

com_ptr<IDirect3DVertexBuffer9> const& vertex_buffer::native() const
{
    return m_native;
}

DWORD vertex_buffer::bytes() const
{
    return m_bytes;
}

void vertex_buffer::before_destruction()
{
    // This vertex buffer might be owned by static object, thus we can
    // outlive the graphics device.
    release_native();
}

void vertex_buffer::before_reset()
{
    if (m_pool == D3DPOOL_DEFAULT)
    {
        release_native();
    }
}

void vertex_buffer::after_reset()
{
    if (m_pool == D3DPOOL_DEFAULT)
    {
        // We dont have source data to recreate D3DPOOL_DEFAULT buffer.
        assert(!native());
        assert(!"todo");
        create_native();
    }
}

void vertex_buffer::create_native()
{
    m_native = create_native_vertex_buffer(device(), m_bytes, m_usage,
        m_fvf, m_pool);
}

void vertex_buffer::create_native(DWORD bytes, DWORD usage, DWORD fvf,
    D3DPOOL pool)
{
    m_bytes = bytes;
    m_usage = usage;
    m_fvf = fvf;
    m_pool = pool;

    create_native();
}

void vertex_buffer::release_native()
{
    m_native.reset();
}

// free functions

com_ptr<IDirect3DVertexBuffer9> create_native_vertex_buffer(
    graphics_device& device, UINT bytes, DWORD usage, DWORD FVF,
    D3DPOOL pool)
{
    com_ptr<IDirect3DVertexBuffer9> vertexbuffer;
    check_dx(device.native()->CreateVertexBuffer(bytes, usage, FVF, pool,
        vertexbuffer.receive(), 0),
        "IDirect3DDevice9::CreateVertexBuffer");
    return vertexbuffer;
}

}} // namespace pt::d3d9
