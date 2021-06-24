#ifndef PT_D3D9_SCOPED_CONVENIENCE_H
#define PT_D3D9_SCOPED_CONVENIENCE_H

#define D3D_DEBUG_INFO

#include "device_child.h"
#include "native.h"
#include "pt/com_ptr.h"

namespace pt { namespace d3d9 {

/// Mechanism for scoped scene.
class scoped_scene : device_reference
{
public:
    /// Calls begin scene.
    scoped_scene(graphics_device& device);
    /// Calls end scene.
    ~scoped_scene();
};

/// Mechanism for scoped render target.
struct scoped_render_target : device_reference
{
public:
    /// Set \a surface as current render target
    scoped_render_target(graphics_device& device,
        com_ptr<IDirect3DSurface9> surface, DWORD index = 0);
    /// Revert back to previous rendertarget.
    ~scoped_render_target();

private:
    com_ptr<IDirect3DSurface9> m_old;
    DWORD m_index;
};

/// Mechanism for scoped depth stencil.
struct scoped_depth_stencil : device_reference
{
    /// Set \a surface as current depth stencil
    scoped_depth_stencil(graphics_device& device,
        com_ptr<IDirect3DSurface9> surface);
    /// Revert back to previous depth stencil.
    ~scoped_depth_stencil();

private:
    com_ptr<IDirect3DSurface9> m_old;
};

/// Sets render state when constructed, and restores old state on destruction.
class scoped_render_state : device_reference
{
public:
    /// Sets render \a state with \a value.
    scoped_render_state(graphics_device& device,
        D3DRENDERSTATETYPE state, DWORD value);
    /// Restores old render state.
    ~scoped_render_state();

private:
    D3DRENDERSTATETYPE m_state;
    DWORD m_old_value;
};

struct scoped_texture_2d_lock
{
    scoped_texture_2d_lock(
        com_ptr<IDirect3DTexture9> texture);
    ~scoped_texture_2d_lock();

    void* data();

private:
    com_ptr<IDirect3DTexture9> m_texture;
    D3DLOCKED_RECT locked_rect;
};

/// Sets texture stage state when constructed, and restores old state on
/// destruction.
class scoped_texture_stage_state : device_reference
{
public:
    /// Sets texture \a stage \a state with \a value.
    scoped_texture_stage_state(graphics_device& device,
        DWORD stage, D3DTEXTURESTAGESTATETYPE state,
        DWORD value);
    /// Restores old texture stage state.
    ~scoped_texture_stage_state();

private:
    DWORD m_stage;
    D3DTEXTURESTAGESTATETYPE m_state;
    DWORD m_old_value;
};

/// Scoped D3D viewport.
class scoped_viewport : device_reference
{
public:
    scoped_viewport(graphics_device& device, D3DVIEWPORT9 const& viewport);
    ~scoped_viewport();

private:
    D3DVIEWPORT9 m_old_viewport;
};

/// Scoped locking and unlocking mechanism for vertex buffers.
class vertex_buffer_lock
{
public:
    /// Performs a lock on \a vb.
    vertex_buffer_lock(com_ptr<IDirect3DVertexBuffer9> buffer,
        UINT offset_to_lock, UINT size_to_lock, DWORD flags);

    /// Performs an unlock on locked vertex buffer.
    ~vertex_buffer_lock();

    /// \returns pointer to locked resource.
    void* data() const;

    template<typename T>
    T* data() const;

private: // noncopyable
    vertex_buffer_lock(vertex_buffer_lock const&);
    vertex_buffer_lock const& operator=(vertex_buffer_lock const&);

private: // data members
    com_ptr<IDirect3DVertexBuffer9> m_buffer;
    void* m_data;
};

template<typename T>
T* vertex_buffer_lock::data() const
{
    return reinterpret_cast<T*>(m_data);
}

}} // namespace pt::d3d9

#endif
