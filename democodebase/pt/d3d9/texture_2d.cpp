#include "texture_2d.h"
#include "texture_2d_builder.h"
#include "enum_utility.h"
#include "error.h"
#include "graphics_device.h"
#include "device_convenience.h"
#include "pt/file_input_stream.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/exceptions/file_error.h"

namespace pt { namespace d3d9 {

// texture_2d

texture_2d::texture_2d(graphics_device& device,
    com_ptr<IDirect3DTexture9> native)
: resource(device)
, m_texture(native)
{
}

texture_2d::texture_2d(texture_2d&& other)
: resource(other.device())
, m_texture(std::move(other.m_texture))
{
}

texture_2d::~texture_2d()
{
    if (!m_texture)
        return;

    ULONG current_reference_count = m_texture->AddRef() - 1;
    m_texture->Release();

    if (current_reference_count == 1)
    {
        D3DSURFACE_DESC desc;
        check_dx(m_texture.get()->GetLevelDesc(0, &desc),
            "IDirect3DTexture9::GetSurfaceLevel");

        size_t bytes = desc.Width * desc.Height
            * format_size_in_bits(desc.Format) / 8;

        log(pt::format("Deallocated texture2d: approximately %1%")
            % bytes_in_formatted_text(bytes));
    }
}

vector2i texture_2d::resolution() const
{
    D3DSURFACE_DESC desc;
    check_dx(m_texture.get()->GetLevelDesc(0, &desc),
        "IDirect3DTexture9::GetSurfaceLevel");

    return vector2i(desc.Width, desc.Height);
}

inline com_ptr<IDirect3DTexture9>& texture_2d::native()
{
    return m_texture;
}

inline com_ptr<IDirect3DTexture9> const& texture_2d::native() const
{
    return m_texture;
}

void texture_2d::before_destruction()
{
    //m_texture.reset();
}

void texture_2d::before_reset()
{
    //m_texture.reset();
}

void texture_2d::after_reset()
{
}

inline com_ptr<IDirect3DSurface9> surface_level(
    texture_2d& texture, size_t level)
{
    return surface_level(texture.native(), static_cast<UINT>(level));
}

inline com_ptr<IDirect3DSurface9> surface_level(
    com_ptr<IDirect3DTexture9>& texture, UINT level)
{
    com_ptr<IDirect3DSurface9> result;
    check_dx(texture->GetSurfaceLevel(level, result.receive()),
        "IDirect3DTexture9::GetSurfaceLevel");
	return result;
}

void set_texture(texture_2d const& texture, size_t sampler_index)
{
    set_texture(texture.mutable_device(),
        static_cast<DWORD>(sampler_index), texture.native());
}

scoped_render_target_texture::scoped_render_target_texture(
    texture_2d& texture)
:   m_texture(texture)
,   m_index(0)
,	m_old(render_target(texture.device(), 0))
{
    set(0);
}

scoped_render_target_texture::scoped_render_target_texture(
    texture_2d& texture, size_t mip_level, DWORD index)
:   m_texture(texture)
,   m_index(index)
//,	m_old(render_target(texture.device(), index))
{
    set(static_cast<UINT>(mip_level));
}

inline void scoped_render_target_texture::set(UINT mip_level)
{
    // TODO assert texture desc has render target flag.
	set_render_target(m_texture.device(),
        surface_level(m_texture, mip_level), m_index);
}

scoped_render_target_texture::~scoped_render_target_texture()
{
    try
    {
		if (m_index == 0)
			set_render_target(m_texture.device(), m_old, m_index);
		else
			m_texture.device().native()->SetRenderTarget(m_index,NULL);
    }
    catch(...)
    {
        log("exception in ~scoped_render_target_texture");
        assert(false);
    }
}

// scoped_optional_rendertarget_texture

scoped_optional_rendertarget_texture::scoped_optional_rendertarget_texture(
	std::shared_ptr<texture_2d> texture)
:   optional_device_reference(texture ? &texture->device() : 0)
,   m_texture(texture)
{
    if (m_texture)
    {
        //graphics_device& device = texture->device();

        m_old = render_target(device(), 0);
        com_ptr<IDirect3DSurface9> surface = surface_level(*texture, 0);
        set_render_target(device(), surface, 0);
    }
}
scoped_optional_rendertarget_texture::~scoped_optional_rendertarget_texture()
{
    if (m_texture)
    {
        try
        {
	        set_render_target(device(), m_old, 0);
        }
        catch(...)
        {
            log("exception in ~scoped_optional_rendertarget");
            assert(false);
        }
    }
}

}} // namespace pt::d3d9
