#ifndef PT_D3D9_VERTEX_BUFFER_H
#define PT_D3D9_VERTEX_BUFFER_H

#include "resource.h"
#include "native.h"
#include "pt/com_ptr.h"

namespace pt { namespace d3d9 {

class vertex_buffer : public resource
{
public:
    vertex_buffer(graphics_device& device, DWORD bytes, DWORD usage,
        DWORD fvf, D3DPOOL pool);
    ~vertex_buffer();

    com_ptr<IDirect3DVertexBuffer9>& native();
    com_ptr<IDirect3DVertexBuffer9> const& native() const;
    DWORD bytes() const;

protected: // interface required by device_child
    void before_destruction();
    void before_reset();
    void after_reset();

private: // helper functions
    void create_native();
    void create_native(DWORD bytes, DWORD usage, DWORD fvf, D3DPOOL pool);
    void release_native();

private: // data members
    com_ptr<IDirect3DVertexBuffer9> m_native;

    DWORD m_fvf;
    DWORD m_bytes;
    DWORD m_usage;
    D3DPOOL m_pool;
};

/// Create vertex buffer. \return Created vertex buffer.
com_ptr<IDirect3DVertexBuffer9> create_native_vertex_buffer(
    graphics_device& device, UINT bytes, DWORD usage, DWORD FVF,
    D3DPOOL pool);

}} // namespace pt::d3d9

#endif
