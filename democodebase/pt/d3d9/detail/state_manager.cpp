#include "state_manager.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/error.h"
#include "pt/any_error.h"
#include "pt/none.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace detail {

namespace {
    DWORD get_render_state(graphics_device& device, D3DRENDERSTATETYPE state)
    {
        DWORD result;
        check_dx(device.native()->GetRenderState(state, &result),
            "IDirect3DDevice9::GetRenderState");
        return result;
    }
    DWORD get_sampler_state(graphics_device& device, DWORD sampler,
        D3DSAMPLERSTATETYPE type)
    {
        DWORD result;
        check_dx(device.native()->GetSamplerState(sampler, type, &result),
            "IDirect3DDevice9::GetSamplerState");
        return result;
    }
    DWORD texture_stage_state(graphics_device const& device, DWORD stage,
        D3DTEXTURESTAGESTATETYPE type)
    {
        DWORD result;
        check_dx(device.mutable_native()->GetTextureStageState(
            stage, type, &result), "IDirect3D9::GetTextureStageState");
        return result;
    }

    com_ptr<IDirect3DSurface9> get_depth_stencil_surface(
        graphics_device const& device)
    {
        com_ptr<IDirect3DSurface9> surface;

        HRESULT hr
            = device.mutable_native()->GetDepthStencilSurface(surface.receive());

        if (hr != D3D_OK)
        {
            if (hr == D3DERR_NOTFOUND)
                return com_ptr<IDirect3DSurface9>();
            check_dx(hr, "IDirect3DDevice9::GetDepthStencilSurface");
        }

        return surface;
    }

    DWORD valid_render_states[] =
        { 7, 8, 9, 14, 15, 16, 19, 20, 22, 23, 24, 25, 26, 27, 28, 29, 34,
        35, 36, 37, 38, 48, 52, 53, 54, 55, 56, 57, 58, 59, 60, 128, 129,
        130, 131, 132, 133, 134, 135, 136, 137, 139, 140, 141, 142, 143, 145,
        146, 147, 148, 151, 152, 154, 155, 156, 157, 158, 159, 160, 161, 162,
        163, 165, 166, 167, 168, 170, 171, 172, 173, 174, 175, 176, 178, 179,
        180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
        194, 195, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209
        };

    DWORD valid_texture_states[] =
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 22, 23, 24, 26, 27, 28, 32 };

    static int const max_sampler_state_types = 14;
    static int const max_samplers = 16;

} // anonymous namespace

// sampler_state_manager

sampler_state_manager::sampler_state_manager()
:   m_index(none<DWORD>())
{
}

void sampler_state_manager::init(graphics_device* device, DWORD index)
{
    m_device = device;
    m_index = index;
    m_states.resize(max_sampler_state_types);

    for (DWORD i = 1; i < max_sampler_state_types; ++i)
    {
        D3DSAMPLERSTATETYPE type = static_cast<D3DSAMPLERSTATETYPE>(i);
        m_states[i].value = get_sampler_state(*device, m_index, type);
        m_states[i].is_dirty = false;
    }
}

void sampler_state_manager::before_draw()
{
    for (DWORD i = 1; i < max_sampler_state_types; ++i)
    {
        D3DSAMPLERSTATETYPE type = static_cast<D3DSAMPLERSTATETYPE>(i);
        if (m_states[type].is_dirty)
        {
            check_dx(m_device->native()->SetSamplerState(m_index, type,
                m_states[type].value), "IDirect3D9::SetSamplerState");

            m_states[type].is_dirty = false;
        }
    }
}

void sampler_state_manager::set_sampler_state(D3DSAMPLERSTATETYPE type,
    DWORD state)
{
    if (state != m_states[type].value)
    {
        m_states[type].value = state;
        m_states[type].is_dirty = true;
    }
}

// texture_stage_state_cache

texture_stage_state_cache::texture_stage_state_cache(graphics_device* device)
:   device_reference(*device)
,   m_possibly_dirty(false)
{
    int const texture_state_count
        = sizeof(valid_texture_states) / sizeof(valid_texture_states[0]);

    for (DWORD stage = 0; stage < 8; ++stage)
    {
        for (size_t i = 0; i < texture_state_count; ++i)
        {
            D3DTEXTURESTAGESTATETYPE type
                = static_cast<D3DTEXTURESTAGESTATETYPE>(
                valid_texture_states[i]);

            basic_state<DWORD> state(
                texture_stage_state(*device, stage, type));

            if (!m_stage_states[stage].insert(
                state_map::value_type(type, state)).second)
            {
                throw any_error(
                    "Failed to create texture_stage_state_cache");
            }
        }
    }
}

void texture_stage_state_cache::before_draw()
{
    if (!m_possibly_dirty)
        return;

    for (DWORD stage = 0; stage < 8; ++stage)
    {
        for (state_map::iterator iter = m_stage_states[stage].begin();
            iter != m_stage_states[stage].end(); ++iter)
        {
            D3DTEXTURESTAGESTATETYPE type = iter->first;
            basic_state<DWORD>& state = iter->second;

            if (state.is_dirty())
            {
                check_dx(device().native()->SetTextureStageState(stage, type,
                    state.get_and_clean()),
                    "IDirect3D9::SetTextureStageState");
            }
        }
    }
    m_possibly_dirty = false;
}

void texture_stage_state_cache::set_texture_stage_state(DWORD stage,
    D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
    m_possibly_dirty = true;
    m_stage_states[stage][type].set(value);
}

DWORD texture_stage_state_cache::get_state(DWORD stage,
    D3DTEXTURESTAGESTATETYPE type) const
{
    state_map::const_iterator i = m_stage_states[stage].find(type);
    assert(i != m_stage_states[stage].end());

    return i->second.get_new();
}

// state_manager

state_manager::state_manager(graphics_device* device)
:   m_device(device)
,   m_texture_states(device)
,   m_render_states_dirty(false)
{
    // Render states
    int const render_state_count
        = sizeof(valid_render_states) / sizeof(valid_render_states[0]);

    m_render_states.resize(render_state_count);
    m_new_render_states.resize(render_state_count);

    for (size_t i = 0; i < render_state_count; ++i)
    {
        D3DRENDERSTATETYPE type
            = static_cast<D3DRENDERSTATETYPE>(valid_render_states[i]);
        DWORD state = get_render_state(*device, type);

        m_render_states[i] = state;
        m_new_render_states[i] = state;

        m_render_state_to_index[type] = i;
    }

    // Sampler states
    m_sampler_states.resize(max_samplers);
    m_textures.resize(max_samplers);
    for (DWORD i = 0; i < max_samplers; ++i)
        m_sampler_states[i].init(device, i);

    m_depth_stencil = com_ptr_state<IDirect3DSurface9>(
        get_depth_stencil_surface(*m_device));

    m_clipping_planes.resize(6);
}

void state_manager::before_draw()
{
    // Render states.
    if (m_render_states_dirty)
    {
        for (render_state_to_index_map::const_iterator i
            = m_render_state_to_index.begin();
            i != m_render_state_to_index.end(); ++i)
        {
            D3DRENDERSTATETYPE type = i->first;
            size_t index = i->second;

            DWORD new_state = m_new_render_states[index];

            if (m_render_states[index] != new_state)
            {
	            check_dx(m_device->native()->SetRenderState(type, new_state),
                    "IDirect3D9::SetRenderState");

                m_render_states[index] = new_state;
            }
        }
        m_render_states_dirty = false;
    }

    // Sampler states.
    for (sampler_states::iterator i = m_sampler_states.begin();
        i != m_sampler_states.end(); ++i)
    {
        i->before_draw();
    }

    if (m_pixel_shader.is_dirty())
    {
        check_dx(m_device->native()->SetPixelShader(
            m_pixel_shader.get_and_clean().get()),
            "IDirect3DDevice9::SetPixelShader");
    }
    if (m_vertex_shader.is_dirty())
    {
        check_dx(m_device->native()->SetVertexShader(
            m_vertex_shader.get_and_clean().get()),
            "IDirect3DDevice9::SetVertexShader");
    }

    // FVF sucks! Setting fvf will "forget" the vertex declaration.
    bool null_fvf = false;
    if (m_fvf.is_dirty())
    {
        DWORD fvf = m_fvf.get_and_clean();

        check_dx(m_device->native()->SetFVF(fvf), "IDirect3D9::SetFVF");
        if (fvf == 0)
            null_fvf = true;
    }
    if (m_vertex_declaration.is_dirty() || null_fvf)
    {
        com_ptr<IDirect3DVertexDeclaration9> const& native
            = m_vertex_declaration.get_and_clean();
        if (native)
        {
            check_dx(m_device->native()->SetVertexDeclaration(native.get()),
                "IDirect3DDevice9::SetVertexDeclaration");
        }
    }

    for (DWORD i = 0; i < max_samplers; ++i)
    {
        if (m_textures[i].is_dirty())
        {
            check_dx(m_device->native()->SetTexture(i,
                m_textures[i].get_and_clean().get()),
                "IDirect3DDevice9::SetTexture");
        }
    }

    for (size_t i = 0; i < 6; ++i)
    {
        if (m_clipping_planes[i].is_dirty())
        {
            check_dx(m_device->native()->SetClipPlane(
                i,
                m_clipping_planes[i].get_and_clean().data()),
                "IDirect3DDevice9::SetClipPlane");
        }
    }

    m_texture_states.before_draw();
}

void state_manager::set_clip_plane(DWORD index, float const* plane)
{
    assert(index < 6);
    auto plane_vector = vector4f(plane[0], plane[1], plane[2], plane[3]);
    m_clipping_planes[index].set(plane_vector);
}

void state_manager::set_render_state(D3DRENDERSTATETYPE state, DWORD value)
{
    size_t index = m_render_state_to_index[state];

    if (m_new_render_states[index] != value)
    {
        m_render_states_dirty = true;
        m_new_render_states[index] = value;
    }
}

DWORD state_manager::render_state(D3DRENDERSTATETYPE state) const
{
    render_state_to_index_map::const_iterator i
        = m_render_state_to_index.find(state);
    assert(i != m_render_state_to_index.end());

    size_t index = i->second;
    return m_new_render_states[index];
}

void state_manager::set_sampler_state(DWORD sampler, D3DSAMPLERSTATETYPE type,
    DWORD state)
{
    assert(sampler < m_sampler_states.size());
    m_sampler_states[sampler].set_sampler_state(type, state);
}

void state_manager::set_pixel_shader(
    com_ptr<IDirect3DPixelShader9> const& pixel_shader)
{
    m_pixel_shader.set(pixel_shader);
}

void state_manager::set_vertex_shader(
    com_ptr<IDirect3DVertexShader9> const& vertex_shader)
{
    m_vertex_shader.set(vertex_shader);
}

void state_manager::set_vertex_declaration(
    com_ptr<IDirect3DVertexDeclaration9> const& declaration)
{
    m_vertex_declaration.set(declaration);
    m_fvf.set(0);
}

void state_manager::set_texture(DWORD sampler,
    com_ptr<IDirect3DTexture9> const& texture)
{
    assert(sampler < m_textures.size());
    m_textures[sampler].set(texture);
}

void state_manager::set_fvf(DWORD fvf)
{
    m_fvf.set(fvf);
    if (fvf != 0)
        m_vertex_declaration.set(com_ptr<IDirect3DVertexDeclaration9>());
}

void state_manager::set_depth_stencil_surface(
    com_ptr<IDirect3DSurface9> const& surface)
{
    m_depth_stencil.set(surface);

    if (m_depth_stencil.is_dirty())
    {
        check_dx(m_device->native()->SetDepthStencilSurface(
            m_depth_stencil.get_and_clean().get()),
            "IDirect3DDevice9::SetDepthStencilSurface");
    }
}

com_ptr<IDirect3DSurface9> state_manager::optional_depth_stencil_surface()
{
    return m_depth_stencil.get();
}

texture_stage_state_cache const& state_manager::texture_states() const
{
    return m_texture_states;
}

texture_stage_state_cache& state_manager::texture_states()
{
    return m_texture_states;
}

}}} // namespace pt::d3d9::detail
