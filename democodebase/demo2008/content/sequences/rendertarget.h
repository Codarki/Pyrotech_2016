#ifndef CONTENT_SEQUENCES_RENDERTARGET_H
#define CONTENT_SEQUENCES_RENDERTARGET_H

#include "pt/d3d9/device_child.h"
#include "pt/d3d9/native.h"
#include "pt/d3d9/texture_2d_fwd.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"

namespace content { namespace sequences {

using namespace pt;

/// Rendertarget to a texture or directly into surface.
class rendertarget : public d3d9::optional_device_reference
{
public:
    rendertarget();
    rendertarget(d3d9::graphics_device& device,
		std::shared_ptr<d3d9::texture_2d> texture);
    rendertarget(d3d9::graphics_device& device,
        com_ptr<IDirect3DSurface9> surface);

    std::shared_ptr<d3d9::texture_2d> texture();
    com_ptr<IDirect3DSurface9> surface();

	void activate(int index = 0);

private: // data members
    std::shared_ptr<d3d9::texture_2d> m_texture;
    com_ptr<IDirect3DSurface9> m_surface;
};

/// Optionally sets rendertarget for the scope.
class scoped_optional_rendertarget
{
public:
    scoped_optional_rendertarget(rendertarget& target, int index = 0);
    ~scoped_optional_rendertarget();

private: // noncopyable
    scoped_optional_rendertarget(scoped_optional_rendertarget const&);
    scoped_optional_rendertarget const& operator=(scoped_optional_rendertarget const&);

private: // data members
    rendertarget& m_target;
    com_ptr<IDirect3DSurface9> m_old;
    int m_index;
};

}} // namespace content::sequences

#endif
