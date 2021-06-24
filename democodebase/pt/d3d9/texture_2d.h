#pragma once
#ifndef PT_D3D9_TEXTURE_2D_H
#define PT_D3D9_TEXTURE_2D_H

#include "resource.h"
#include "native.h"
#include "pt/com_ptr.h"
#include "pt/vector.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {

class texture_2d : public resource
{
public:
    texture_2d(graphics_device& device, com_ptr<IDirect3DTexture9> native);
    texture_2d(texture_2d&& other);
    ~texture_2d();

    vector2i resolution() const;
    com_ptr<IDirect3DTexture9>& native();
    com_ptr<IDirect3DTexture9> const& native() const;

public: // interface required by resource
    void before_destruction();
    void before_reset();
    void after_reset();

private: // data members
    com_ptr<IDirect3DTexture9> m_texture;
};

com_ptr<IDirect3DSurface9> surface_level(texture_2d& texture, size_t level);
com_ptr<IDirect3DSurface9> surface_level(com_ptr<IDirect3DTexture9>& texture,
    UINT level);

/// Sets \a texture to \a sampler_index.
void set_texture(texture_2d const& texture, size_t sampler_index);

/// Scoped render target into texture_2d.
class scoped_render_target_texture
{
public:
    /// Sets \a texture's first mip level at render target 0.
    scoped_render_target_texture(texture_2d& texture);
    /// Sets \a texture's \a mip_level at render target \a index.
    scoped_render_target_texture(texture_2d& texture, size_t mip_level,
        DWORD index);
    /// Restores previous render target.
    ~scoped_render_target_texture();

private: // helper functions
    void set(UINT mip_level);

private: // noncopyable
    scoped_render_target_texture(scoped_render_target_texture const&);
    scoped_render_target_texture const& operator=(scoped_render_target_texture const&);

private: // data members
    texture_2d& m_texture;
    DWORD m_index;
    com_ptr<IDirect3DSurface9> m_old;
};

class scoped_optional_rendertarget_texture : optional_device_reference
{
public:
	scoped_optional_rendertarget_texture(std::shared_ptr<texture_2d> texture);
    ~scoped_optional_rendertarget_texture();

private: // data members
	std::shared_ptr<d3d9::texture_2d> m_texture;
    com_ptr<IDirect3DSurface9> m_old;
};

}} // namespace pt::d3d9

#endif
