#include "device_convenience.h"
#include "detail/state_manager.h"
#include "detail/vertex_declaration_cache.h"
#include "graphics_device.h"
#include "error.h"

namespace pt { namespace d3d9 {

namespace detail {
    void get_render_target(graphics_device const& device, DWORD index,
        com_ptr<IDirect3DSurface9>& result)
    {
        check_dx(device.mutable_native()->GetRenderTarget(
            index, result.receive()),
            "IDirect3DDevice9::GetRenderTarget");
    }
} // namespace detail

void begin_scene(graphics_device& device)
{
    check_dx(device.native()->BeginScene(), "IDirect3DDevice9::BeginScene");
}

void end_scene(graphics_device& device)
{
    check_dx(device.native()->EndScene(), "IDirect3DDevice9::EndScene");
}

void clear(graphics_device& device, DWORD flags, vector4f const& color,
    float z, DWORD stencil)
{
    D3DCOLOR d3d_color
        = D3DCOLOR_COLORVALUE(color[0], color[1], color[2], color[3]);

    clear(device, 0, NULL, flags, d3d_color, z, stencil);
}

void clear(graphics_device& device, DWORD count, D3DRECT const* rects,
    DWORD flags, D3DCOLOR color, float z, DWORD stencil)
{
    check_dx(device.native()->Clear(count, rects, flags, color, z, stencil),
        "IDirect3D9::Clear");
}

void set_clip_plane(graphics_device& device, DWORD index, float const* plane)
{
    device.state_manager().set_clip_plane(index, plane);
}

com_ptr<IDirect3DSurface9> create_depth_stencil_surface(
    graphics_device const& device, UINT width, UINT height,
    D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample,
    DWORD multi_sample_quality, bool discard)
{
    com_ptr<IDirect3DSurface9> result;
    check_dx(device.mutable_native()->CreateDepthStencilSurface(
        width, height, format, multi_sample, multi_sample_quality, discard,
        result.receive(), 0),
        "IDirect3DDevice9::CreateDepthStencilSurface");
    return result;
}

com_ptr<IDirect3DSurface9> create_offscreen_plain_surface(
    graphics_device const& device, UINT width, UINT height,
    D3DFORMAT format, D3DPOOL pool)
{
    com_ptr<IDirect3DSurface9> result;
    check_dx(device.mutable_native()->CreateOffscreenPlainSurface(
        width, height, format, pool, result.receive(), 0),
        "IDirect3DDevice9::CreateOffscreenPlainSurface");
    return result;
}

com_ptr<IDirect3DTexture9> create_texture(graphics_device const& device,
    UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format,
    D3DPOOL pool)
{
    com_ptr<IDirect3DTexture9> result;
    check_dx(device.mutable_native()->CreateTexture(width, height, levels,
        usage, format, pool, result.receive(), 0),
        "IDirect3DDevice9::CreateTexture");
    return result;
}

void update_texture(graphics_device const& device,
    com_ptr<IDirect3DTexture9> source, com_ptr<IDirect3DTexture9> target)
{
    check_dx(device.mutable_native()->UpdateTexture(source.get(), target.get()),
        "IDirect3DDevice9::UpdateTexture");
}

com_ptr<IDirect3DVertexDeclaration9> create_vertex_declaration(
    graphics_device const& device, D3DVERTEXELEMENT9 const* elements)
{
    return device.vertex_declarations().find_or_create(elements);
    //com_ptr<IDirect3DVertexDeclaration9> result;
    //check_dx(device.mutable_native()->CreateVertexDeclaration(elements,
    //    result.receive()),
    //    "IDirect3DDevice9::CreateOffscreenPlainSurface");
    //return result;
}

com_ptr<IDirect3DPixelShader9> create_pixel_shader(
    graphics_device const& device, DWORD const* function)
{
    com_ptr<IDirect3DPixelShader9> result;
    check_dx(device.mutable_native()->CreatePixelShader(
        function, result.receive()),
        "IDirect3DDevice9::CreatePixelShader");
    return result;
}

com_ptr<IDirect3DVertexShader9> create_vertex_shader(
    graphics_device const& device, DWORD const* function)
{
    com_ptr<IDirect3DVertexShader9> result;
    check_dx(device.mutable_native()->CreateVertexShader(
        function, result.receive()),
        "IDirect3DDevice9::CreateVertexShader");
    return result;
}

void draw_indexed_primitive(graphics_device& device, D3DPRIMITIVETYPE type,
    INT base_vertex_index, UINT min_index, UINT num_vertices,
    UINT start_index, UINT primitive_count)
{
    device.state_manager().before_draw();
    check_dx(device.native()->DrawIndexedPrimitive(type,
        base_vertex_index, min_index, num_vertices, start_index,
        primitive_count),
        "IDirect3D9::DrawIndexedPrimitive");
}

void draw_primitive(graphics_device& device, D3DPRIMITIVETYPE type,
    UINT start_vertex, UINT primitive_count)
{
    device.state_manager().before_draw();
    check_dx(device.native()->DrawPrimitive(type, start_vertex,
        primitive_count), "IDirect3D9::DrawPrimitive");
}

void front_buffer_data(graphics_device const& device, UINT swap_chain,
    com_ptr<IDirect3DSurface9> destination)
{
    check_dx(device.mutable_native()->GetFrontBufferData(swap_chain,
        destination.get()), "IDirect3DDevice9::GetFrontBufferData");
}

void get_display_mode(graphics_device const& device, UINT swap_chain,
    D3DDISPLAYMODE& mode)
{
    check_dx(device.mutable_native()->GetDisplayMode(swap_chain, &mode),
        "IDirect3DDevice9::GetDisplayMode");
}

void light_enable(graphics_device& device, DWORD light_index, bool enable)
{
    check_dx(device.native()->LightEnable(light_index, enable),
        "IDirect3D9::LightEnable");
}

void present(graphics_device const& device, RECT const* source_rect,
    RECT const* dest_rect, HWND dest_window_override,
    RGNDATA const* dirty_region)
{
    check_dx(device.mutable_native()->Present(source_rect, dest_rect,
        dest_window_override, dirty_region), "IDirect3D9::Present");
}

void set_depth_stencil_surface(graphics_device& device,
    com_ptr<IDirect3DSurface9> const& surface)
{
    device.state_manager().set_depth_stencil_surface(surface);
    //check_dx(device.native()->SetDepthStencilSurface(surface.get()),
    //    "IDirect3DDevice9::SetDepthStencilSurface");
}

com_ptr<IDirect3DSurface9> depth_stencil_surface(
    graphics_device& device)
{
    return device.state_manager().optional_depth_stencil_surface();
    //com_ptr<IDirect3DSurface9> surface;
    //check_dx(device.mutable_native()->GetDepthStencilSurface(surface.receive()),
    //    "IDirect3DDevice9::GetDepthStencilSurface");
    //return surface;
}

com_ptr<IDirect3DSurface9> optional_depth_stencil_surface(
    graphics_device& device)
{
    return device.state_manager().optional_depth_stencil_surface();
    //com_ptr<IDirect3DSurface9> surface;

    //HRESULT hr
    //    = device.mutable_native()->GetDepthStencilSurface(surface.receive());

    //if (hr != D3D_OK)
    //{
    //    if (hr == D3DERR_NOTFOUND)
    //        return com_ptr<IDirect3DSurface9>();
    //    check_dx(hr, "IDirect3DDevice9::GetDepthStencilSurface");
    //}

    //return surface;
}

void set_fvf(graphics_device& device, DWORD fvf)
{
    device.state_manager().set_fvf(fvf);
    //check_dx(device.native()->SetFVF(fvf), "IDirect3D9::SetFVF");
}

void set_indices(graphics_device& device,
    com_ptr<IDirect3DIndexBuffer9> const& ib)
{
    check_dx(device.native()->SetIndices(ib.get()), "IDirect3D9::SetIndices");
}

void set_light(graphics_device& device, DWORD index, D3DLIGHT9 const* light)
{
    check_dx(device.native()->SetLight(index, light), "IDirect3D9::SetLight");
}

void set_pixel_shader(graphics_device& device,
    com_ptr<IDirect3DPixelShader9> const& pixel_shader)
{
    device.state_manager().set_pixel_shader(pixel_shader);
    //check_dx(device.native()->SetPixelShader(pixel_shader.get()),
    //    "IDirect3DDevice9::SetPixelShader");
}

void set_render_state(graphics_device& device, D3DRENDERSTATETYPE state,
    DWORD value)
{
    device.state_manager().set_render_state(state, value);
    //check_dx(device.native()->SetRenderState(state, value),
    //    "IDirect3D9::SetRenderState");
}

DWORD render_state(graphics_device const& device, D3DRENDERSTATETYPE state)
{
    return device.state_manager().render_state(state);
    //DWORD result;
    //check_dx(device.mutable_native()->GetRenderState(state, &result),
    //    "IDirect3D9::GetRenderState");
    //return result;
}

void set_render_target(graphics_device& device,
    com_ptr<IDirect3DSurface9> const& surface, DWORD index)
{
    check_dx(device.native()->SetRenderTarget(index, surface.get()),
        "IDirect3DDevice9::SetRenderTarget");
}

void set_sampler_state(graphics_device& device, DWORD sampler,
    D3DSAMPLERSTATETYPE type, DWORD state)
{
    device.state_manager().set_sampler_state(sampler, type, state);
    //check_dx(device.native()->SetSamplerState(sampler, type, state),
    //    "IDirect3D9::SetSamplerState");
}

void set_stream_source(graphics_device& device, UINT stream_number,
    com_ptr<IDirect3DVertexBuffer9> const& vb, UINT offset_in_bytes,
    UINT stride)
{
    check_dx(device.native()->SetStreamSource(stream_number, vb.get(),
        offset_in_bytes, stride), "IDirect3D9::SetStreamSource");
}

void set_texture(graphics_device& device, DWORD sampler,
    com_ptr<IDirect3DTexture9> const& texture)
{
    device.state_manager().set_texture(sampler, texture);
    //check_dx(device.native()->SetTexture(sampler, texture.get()),
    //    "IDirect3DDevice9::SetTexture");
}

void set_texture_stage_state(graphics_device& device, DWORD stage,
    D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
    device.state_manager().texture_states().set_texture_stage_state(
        stage, type, value);
    //check_dx(device.native()->SetTextureStageState(stage, type, value),
    //    "IDirect3D9::SetTextureStageState");
}

DWORD texture_stage_state(graphics_device const& device, DWORD stage,
    D3DTEXTURESTAGESTATETYPE type)
{
    return device.state_manager().texture_states().get_state(stage, type);
    //DWORD result;
    //check_dx(device.mutable_native()->GetTextureStageState(
    //    stage, type, &result), "IDirect3D9::GetTextureStageState");
    //return result;
}

void set_transform(graphics_device& device, D3DTRANSFORMSTATETYPE state,
    D3DMATRIX const* matrix)
{
    check_dx(device.native()->SetTransform(state, matrix),
        "IDirect3D9::SetTransform");
}

void set_vertex_declaration(graphics_device& device,
    com_ptr<IDirect3DVertexDeclaration9> const& vertex_declaration)
{
    device.state_manager().set_vertex_declaration(vertex_declaration);
    //check_dx(device.native()->SetVertexDeclaration(vertex_declaration.get()),
    //    "IDirect3DDevice9::SetVertexDeclaration");
}

void set_vertex_shader(graphics_device& device,
    com_ptr<IDirect3DVertexShader9> const& vertex_shader)
{
    device.state_manager().set_vertex_shader(vertex_shader);
    //check_dx(device.native()->SetVertexShader(vertex_shader.get()),
    //    "IDirect3DDevice9::SetVertexShader");
}

void set_viewport(graphics_device& device, D3DVIEWPORT9 const& viewport)
{
    check_dx(device.native()->SetViewport(&viewport),
        "IDirect3DDevice9::SetViewport");
}

D3DVIEWPORT9 viewport(graphics_device const& device)
{
    D3DVIEWPORT9 view;
    check_dx(device.mutable_native()->GetViewport(&view),
        "IDirect3DDevice9::GetViewport");
    return view;
}

}} // namespace pt::d3d9
