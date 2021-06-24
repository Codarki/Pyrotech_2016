#include "index_buffer.h"
#include "device_convenience.h"
#include "graphics_device.h"
#include "error.h"
#include "pt/any_error.h"
#include "pt/basic_types.h"
#include "pt/log.h"

namespace pt { namespace d3d9 {

namespace {
    com_ptr<IDirect3DIndexBuffer9> create_native_index_buffer(
        graphics_device& device, UINT bytes, DWORD usage, D3DFORMAT format,
        D3DPOOL pool)
    {
        com_ptr<IDirect3DIndexBuffer9> indexbuffer;
        check_dx(device.native()->CreateIndexBuffer(bytes, usage, format,
            pool, indexbuffer.receive(), NULL),
            "IDirect3DDevice9::CreateIndexBuffer");
        return indexbuffer;
    }
}

index_buffer::index_buffer(graphics_device& device, UINT bytes,
    DWORD usage, D3DFORMAT format, D3DPOOL pool)
:   resource(device)
{
    m_native = create_native_index_buffer(device, bytes, usage, format, pool);

    if (format == D3DFMT_INDEX16)
        m_count = bytes / sizeof(uint16);
    else if (format == D3DFMT_INDEX32)
        m_count = bytes / sizeof(uint32);
    else
        throw any_error("unknown index format");
}

com_ptr<IDirect3DIndexBuffer9>& index_buffer::native()
{
    return m_native;
}

com_ptr<IDirect3DIndexBuffer9> const& index_buffer::native() const
{
    return m_native;
}

void index_buffer::use() const
{
    set_indices(mutable_device(), m_native);
}

size_t index_buffer::count() const
{
    return m_count;
}

void index_buffer::before_destruction()
{
    //assert(!"todo");
}

void index_buffer::before_reset()
{
    D3DINDEXBUFFER_DESC desc;
    check_dx(m_native->GetDesc(&desc), "IDirect3DIndexBuffer9::GetDesc");
    if (desc.Pool == D3DPOOL_DEFAULT)
    {
        assert(!"todo");
    }
}

void index_buffer::after_reset()
{
    //assert(!"todo");
}

// scoped_index_buffer_lock

scoped_index_buffer_lock::scoped_index_buffer_lock(index_buffer& buffer)
:   m_buffer(buffer)
,   m_data(0)
{
    UINT offset = 0;
    UINT size_to_lock = 0;
    DWORD flags = 0;

    check_dx(m_buffer.native()->Lock(offset, size_to_lock, &m_data, flags),
        "IDirect3DIndexBuffer9::Lock");
}

scoped_index_buffer_lock::~scoped_index_buffer_lock()
{
    HRESULT hr = m_buffer.native()->Unlock();
    if (hr != D3D_OK)
        log("IDirect3DIndexBuffer9::Unlock failed in destructor");
}

uint16* scoped_index_buffer_lock::data()
{
    assert(m_data);
    return reinterpret_cast<uint16*>(m_data);
}

// free functions

index_buffer create_default_16bit_index_buffer(graphics_device& device,
    UINT bytes)
{
    DWORD usage = D3DUSAGE_WRITEONLY;
    return index_buffer(device, bytes, usage, D3DFMT_INDEX16, D3DPOOL_DEFAULT);
}

std::unique_ptr<index_buffer> create_default_16bit_unique_index_buffer(
    graphics_device& device,
    UINT bytes)
{
    DWORD usage = D3DUSAGE_WRITEONLY;
    return std::unique_ptr<index_buffer>(
        new index_buffer(device, bytes, usage, D3DFMT_INDEX16, D3DPOOL_DEFAULT));
}


}} // namespace pt::d3d9
