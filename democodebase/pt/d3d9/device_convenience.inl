#ifndef PT_D3D9_DEVICE_CONVENIENCE_INL
#define PT_D3D9_DEVICE_CONVENIENCE_INL

namespace pt { namespace d3d9 {

// Inlinable functions, and versions which wont add COM reference counts.
// This is just eye candy, since result by argument reference is such an
// ugly syntax. Also hides bunch of include dependencies.

namespace detail {
    void get_render_target(graphics_device const& device, DWORD index,
        com_ptr<IDirect3DSurface9>& result);
} // namespace detail

inline com_ptr<IDirect3DSurface9> render_target(
    graphics_device const& device, DWORD index)
{
    com_ptr<IDirect3DSurface9> surface;
    detail::get_render_target(device, index, surface);
    return surface;
}

}} // namespace pt::d3d9

#endif
