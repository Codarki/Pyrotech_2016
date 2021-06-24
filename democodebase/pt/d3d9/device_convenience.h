#ifndef PT_D3D9_DEVICE_CONVENIENCE_H
#define PT_D3D9_DEVICE_CONVENIENCE_H

#define D3D_DEBUG_INFO

#include "native.h"
#include "graphics_device_fwd.h"
#include "pt/com_ptr.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 {

// Convenience direct3d device functions.

void begin_scene(graphics_device& device);
void end_scene(graphics_device& device);

void clear(graphics_device& device, DWORD flags, vector4f const& color,
    float z, DWORD stencil);
void clear(graphics_device& device, DWORD count, const D3DRECT *rects,
    DWORD flags, D3DCOLOR color, float z, DWORD stencil);

void set_clip_plane(graphics_device& device, DWORD index, float const* plane);

com_ptr<IDirect3DSurface9> create_depth_stencil_surface(
    graphics_device const& device, UINT width, UINT height,
    D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample,
    DWORD multi_sample_quality, bool discard);
com_ptr<IDirect3DSurface9> create_offscreen_plain_surface(
    graphics_device const& device, UINT width, UINT height,
    D3DFORMAT format, D3DPOOL pool);

com_ptr<IDirect3DTexture9> create_texture(graphics_device const& device,
    UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format,
    D3DPOOL pool);

void update_texture(graphics_device const& device,
    com_ptr<IDirect3DTexture9> source, com_ptr<IDirect3DTexture9> target);

com_ptr<IDirect3DVertexDeclaration9> create_vertex_declaration(
    graphics_device const& device, D3DVERTEXELEMENT9 const* elements);

com_ptr<IDirect3DPixelShader9> create_pixel_shader(
    graphics_device const& device, DWORD const* function);
com_ptr<IDirect3DVertexShader9> create_vertex_shader(
    graphics_device const& device, DWORD const* function);

void draw_indexed_primitive(graphics_device& device, D3DPRIMITIVETYPE type,
    INT base_vertex_index, UINT min_index, UINT num_vertices,
    UINT start_index, UINT primitive_count);
void draw_primitive(graphics_device& device, D3DPRIMITIVETYPE type,
    UINT start_vertex, UINT primitive_count);

void front_buffer_data(graphics_device const& device, UINT swap_chain,
    com_ptr<IDirect3DSurface9> destination);
void get_display_mode(graphics_device const& device, UINT swap_chain,
    D3DDISPLAYMODE& mode);

void light_enable(graphics_device& device, DWORD light_index, bool enable);

void present(graphics_device const& device, RECT const *source_rect = 0,
    RECT const* dest_rect = 0, HWND dest_window_override = 0,
    RGNDATA const* dirty_region = 0);

void set_depth_stencil_surface(graphics_device& device,
    com_ptr<IDirect3DSurface9> const& surface);
com_ptr<IDirect3DSurface9> depth_stencil_surface(
    graphics_device& device);
com_ptr<IDirect3DSurface9> optional_depth_stencil_surface(
    graphics_device& device);

void set_fvf(graphics_device& device, DWORD fvf);
void set_indices(graphics_device& device,
    com_ptr<IDirect3DIndexBuffer9> const& ib);
void set_light(graphics_device& device, DWORD index, D3DLIGHT9 const* light);
void set_pixel_shader(graphics_device& device,
    com_ptr<IDirect3DPixelShader9> const& pixel_shader);
void set_render_state(graphics_device& device, D3DRENDERSTATETYPE state,
    DWORD value);
DWORD render_state(graphics_device const& device, D3DRENDERSTATETYPE state);

void set_render_target(graphics_device& device,
    com_ptr<IDirect3DSurface9> const& surface, DWORD index);
com_ptr<IDirect3DSurface9> render_target(graphics_device const& device,
    DWORD index);

void set_sampler_state(graphics_device& device, DWORD sampler,
    D3DSAMPLERSTATETYPE type, DWORD state);

void set_stream_source(graphics_device& device, UINT stream_number,
    com_ptr<IDirect3DVertexBuffer9> const& vb, UINT offset_in_bytes,
    UINT stride);

void set_texture(graphics_device& device, DWORD sampler,
    com_ptr<IDirect3DTexture9> const& texture);
void set_texture_stage_state(graphics_device& device, DWORD stage,
    D3DTEXTURESTAGESTATETYPE type, DWORD value);
DWORD texture_stage_state(graphics_device const& device, DWORD stage,
    D3DTEXTURESTAGESTATETYPE type);

void set_transform(graphics_device& device, D3DTRANSFORMSTATETYPE state,
    D3DMATRIX const* matrix);
void set_vertex_declaration(graphics_device& device,
    com_ptr<IDirect3DVertexDeclaration9> const& vertex_declaration);
void set_vertex_shader(graphics_device& device,
    com_ptr<IDirect3DVertexShader9> const& vertex_shader);

void set_viewport(graphics_device& device, D3DVIEWPORT9 const& viewport);
D3DVIEWPORT9 viewport(graphics_device const& device);

}} // namespace pt::d3d9

#include "device_convenience.inl"

#endif
