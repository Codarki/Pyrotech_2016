#ifndef PT_D3D9_FONT_H
#define PT_D3D9_FONT_H

#include "device_child.h"
#include "native.h"
#include "graphics_device_fwd.h"
#include "pt/com_ptr.h"
#include "pt/box.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {

class font : public device_child
{
public:
    font(graphics_device& device, com_ptr<ID3DXFont> const& font);
    ~font();

    com_ptr<ID3DXFont> const& native() const;

protected:
    void before_destruction();
    void before_reset();
    void after_reset();

private: // data members
    com_ptr<ID3DXFont> m_font;
};

std::shared_ptr<font> create_font(graphics_device& device);

/// Draws \a text with given \a font inside \a rect using \a flags. If
/// \a flags is 0, default alignment is top left.
void draw_text(font const& font, std::string const& text,
    RECT& rect, vector4i const& rgba, DWORD flags);

void draw_text(font const& font, std::string const& text,
    box2i const& area, vector4i const& rgba, int flags);

/// Returns the area required to draw \a text.
box2i required_area(font const& font, std::string const& text,
    box2i const& area);

/// Returns native D3DX font.
com_ptr<ID3DXFont> create_d3dx_font(graphics_device& device,
    std::string const& name, int height);

}} // namespace pt::d3d9

#endif
