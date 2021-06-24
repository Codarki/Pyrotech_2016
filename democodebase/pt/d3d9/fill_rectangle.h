#ifndef PT_D3D9_FILL_RECTANGLE_H
#define PT_D3D9_FILL_RECTANGLE_H

#include "native.h"
#include "graphics_device_fwd.h"
#include "pt/com_ptr.h"
#include "pt/box.h"
#include "pt/vector.h"

namespace pt { namespace d3d9 {

/// Fills rectangle defined by \a min and \a max corners with \a texture.
void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, com_ptr<IDirect3DTexture9> texture);

void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, com_ptr<IDirect3DTexture9> texture,
    bool alpha_blend);

/// Fills rectangle defined by \a min and \a max corners with \a texture.
void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector2f const& min_uv, vector2f const& max_uv,
    com_ptr<IDirect3DTexture9> texture, bool alpha_blend);

/// Fills rectangle defined by \a min and \a max corners with \a texture
/// modulated with rgba \a color.
void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector4f const& color,
    com_ptr<IDirect3DTexture9> texture, bool alpha_blend);

/// Fills rectangle defined by \a area with rgba \a color.
void fill_rectangle(graphics_device& device, box2f const& area,
    vector4f const& color);
//template<typename T>
//void fill_rectangle(box<T,2> const& area, vector4f const& color);

/// Fills rectangle defined by \a min and \a max corners with rgba \a color.
void fill_rectangle(graphics_device& device, vector2f const& min,
    vector2f const& max, vector4f const& color);

/// \todo This doesnt fit in fill_rectangle.
void draw_line(graphics_device& device, vector2f const& from,
    vector2f const& to, vector4f const& color);

}} // namespace pt::d3d9

#endif
