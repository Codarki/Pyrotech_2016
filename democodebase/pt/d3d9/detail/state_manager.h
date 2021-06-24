#ifndef PT_D3D9_DETAIL_STATE_MANAGER_H
#define PT_D3D9_DETAIL_STATE_MANAGER_H

#include "../device_child.h"
#include "../native.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/vector.h"
#include "pt/com_ptr.h"
#include "pt/std/map.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace detail {

class sampler_state_manager
{
public:
    sampler_state_manager();
    void init(graphics_device* device, DWORD index);

    void before_draw();
    void set_sampler_state(D3DSAMPLERSTATETYPE type, DWORD state);

private:
    struct state
    {
        DWORD value;
        bool is_dirty;
    };
private:
    graphics_device* m_device;
    DWORD m_index;
    std::vector<state> m_states;
};

template<typename T>
class com_ptr_state
{
public:
    com_ptr_state();
    com_ptr_state(com_ptr<T> const& ptr);

    void set(com_ptr<T> const& ptr);
    bool is_dirty() const;
    com_ptr<T> const& get_and_clean();
    com_ptr<T> const& get();

private:
    bool m_is_dirty;
    com_ptr<T> m_ptr;
};

template<typename T>
class basic_state
{
public:
    basic_state();
    basic_state(T const& initial_value);

    void set(T value);
    bool is_dirty() const;
    T get_and_clean();
    T get_new() const;

private:
    bool m_is_dirty;
    T m_new_value;
    T m_current_value;
};

class texture_stage_state_cache : device_reference
{
public:
    texture_stage_state_cache(graphics_device* device);

    void before_draw();
    void set_texture_stage_state(DWORD stage, D3DTEXTURESTAGESTATETYPE type,
        DWORD value);
    DWORD get_state(DWORD stage, D3DTEXTURESTAGESTATETYPE type) const;

private: // type definitions
    typedef std::map<D3DTEXTURESTAGESTATETYPE,basic_state<DWORD> > state_map;

private: // data members
    state_map m_stage_states[8];
    bool m_possibly_dirty;
};

/// Removes redundant device changes. Implementation detail.
class state_manager
{
public:
public:
    state_manager(graphics_device* device);

    void before_draw();

    void set_clip_plane(DWORD index, float const* plane);

    void set_render_state(D3DRENDERSTATETYPE state, DWORD value);
    DWORD render_state(D3DRENDERSTATETYPE state) const;

    void set_sampler_state(DWORD sampler, D3DSAMPLERSTATETYPE type,
        DWORD state);

    void set_pixel_shader(
        com_ptr<IDirect3DPixelShader9> const& pixel_shader);
    void set_vertex_shader(
        com_ptr<IDirect3DVertexShader9> const& vertex_shader);
    void set_vertex_declaration(
        com_ptr<IDirect3DVertexDeclaration9> const& declaration);
    void set_texture(DWORD sampler,
        com_ptr<IDirect3DTexture9> const& texture);
    void set_fvf(DWORD fvf);

    void set_depth_stencil_surface(
        com_ptr<IDirect3DSurface9> const& surface);
    com_ptr<IDirect3DSurface9> optional_depth_stencil_surface();

    texture_stage_state_cache const& texture_states() const;
    texture_stage_state_cache& texture_states();

private: // type definitions
    typedef std::map<D3DRENDERSTATETYPE,size_t> render_state_to_index_map;
    typedef std::vector<sampler_state_manager> sampler_states;

private: // data members
    graphics_device* m_device;

    std::vector<DWORD> m_render_states;
    std::vector<DWORD> m_new_render_states;
    bool m_render_states_dirty;
    render_state_to_index_map m_render_state_to_index;

    com_ptr_state<IDirect3DSurface9> m_depth_stencil;
    com_ptr_state<IDirect3DPixelShader9> m_pixel_shader;
    com_ptr_state<IDirect3DVertexShader9> m_vertex_shader;
    com_ptr_state<IDirect3DVertexDeclaration9> m_vertex_declaration;
    std::vector<com_ptr_state<IDirect3DTexture9> > m_textures;
    basic_state<DWORD> m_fvf;

    sampler_states m_sampler_states;
    texture_stage_state_cache m_texture_states;

    std::vector<basic_state<vector4f>> m_clipping_planes;
};

// .inl stuff

template<typename T>
inline com_ptr_state<T>::com_ptr_state()
:   m_is_dirty(false)
{
}

template<typename T>
inline com_ptr_state<T>::com_ptr_state(com_ptr<T> const& ptr)
:   m_ptr(ptr)
,   m_is_dirty(false)
{
}

template<typename T>
inline void com_ptr_state<T>::set(com_ptr<T> const& ptr)
{
    if (m_ptr != ptr)
    {
        m_ptr = ptr;
        m_is_dirty = true;
    }
}

template<typename T>
inline bool com_ptr_state<T>::is_dirty() const
{
    return m_is_dirty;
}

template<typename T>
inline com_ptr<T> const& com_ptr_state<T>::get_and_clean()
{
    m_is_dirty = false;
    return m_ptr;
}

template<typename T>
inline com_ptr<T> const& com_ptr_state<T>::get()
{
    return m_ptr;
}

// basic_state<T>

template<typename T>
inline basic_state<T>::basic_state()
:   m_is_dirty(false)
{
}

template<typename T>
inline basic_state<T>::basic_state(T const& initial_value)
:   m_is_dirty(false)
,   m_current_value(initial_value)
,   m_new_value(initial_value)
{
}

template<typename T>
inline void basic_state<T>::set(T value)
{
    m_new_value = value;
}

template<typename T>
inline bool basic_state<T>::is_dirty() const
{
    return m_current_value != m_new_value;
}

template<typename T>
inline T basic_state<T>::get_and_clean()
{
    m_current_value = m_new_value;
    return m_current_value;
}

template<typename T>
inline T basic_state<T>::get_new() const
{
    return m_new_value;
}

}}} // namespace pt::d3d9::detail

#endif
