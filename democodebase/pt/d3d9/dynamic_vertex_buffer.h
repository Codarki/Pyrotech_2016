#ifndef PT_D3D9_DYNAMIC_VERTEX_BUFFER_H
#define PT_D3D9_DYNAMIC_VERTEX_BUFFER_H

#include "device_child.h"
#include "vertex_buffer.h"
#include "graphics_device_fwd.h"
#include "pt/std/map.h"

namespace pt { namespace d3d9 {

namespace detail {
    class static_dynamic_vertex_buffers;
}

/// Reserves block of memory from dynamic D3D vertex buffer.
class dynamic_vertex_buffer : device_reference
{
public:
    dynamic_vertex_buffer(graphics_device& device, DWORD bytes, DWORD fvf,
        DWORD lock_flags);
    ~dynamic_vertex_buffer();

    template<typename T>
    T* data();

    void use(UINT stream_number, UINT stride);

    static void ensure_lenght(graphics_device& device, DWORD bytes,
        DWORD fvf);
    static void clean_up();

private: // typedefs
    friend detail::static_dynamic_vertex_buffers;
    struct buffer
    {
        buffer(graphics_device& device, DWORD bytes, DWORD fvf);

        DWORD current_position;
        bool in_use;
        bool locked;

        vertex_buffer vertex_buffer;
    };

private: // helper functions
    void unlock();

private: // data members
    buffer* m_buffer;
    void* m_data;
    UINT m_start_offset;
    DWORD m_fvf;
};

template<typename T>
T* dynamic_vertex_buffer::data()
{
    return reinterpret_cast<T*>(m_data);
}

}} // namespace pt::d3d9

#endif
