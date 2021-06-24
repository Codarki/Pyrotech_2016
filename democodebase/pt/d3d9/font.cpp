#include "font.h"
#include "error.h"
#include "graphics_device.h"
#include "pt/debug_only.h"
#include "pt/boost/bind.h"
#include "pt/win32/font.h"
#include "pt/win32/font_utility.h"
#include "pt/win32/utility.h"

namespace pt { namespace d3d9 {

namespace {
    com_ptr<ID3DXFont> create_d3dx_font(graphics_device& device)
    {
        win32::log_font_container fonts
            = win32::enum_font(ANSI_CHARSET, "Tahoma");
        assert(!fonts.empty());

        INT height = 13;
        UINT width = 0;//6;
        UINT weight = 0;
        UINT mip_levels = D3DX_DEFAULT;
        BOOL italic = FALSE;
        DWORD char_set = ANSI_CHARSET;
        DWORD output_precision = OUT_TT_ONLY_PRECIS;
        DWORD raster_font_quality = 0;
        DWORD pitch_and_family = 0;
        LPCTSTR face_name = "Tahoma";
        com_ptr<ID3DXFont> font;

        check_dx(D3DXCreateFont(device.native().get(),
            height, width, weight, mip_levels, italic, char_set,
            output_precision, raster_font_quality, pitch_and_family,
            face_name, font.receive()), "D3DXCreateFont");

        return font;
    }

    /// Draws \a text with given \a font inside \a rect using \a flags. If
    /// \a flags is 0, default alignment is top left.
    int draw_native_text(com_ptr<ID3DXFont> font, std::string const& text,
        RECT& rect, vector4i const& rgba, DWORD flags)
    {
        assert(font);
        if (text.empty())
            return -1;

        LPD3DXSPRITE sprite = 0;
        LPCTSTR string = text.c_str();
        INT count = static_cast<INT>(text.length());

        //D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);
        D3DCOLOR color = D3DCOLOR_RGBA(rgba[0], rgba[1], rgba[2], rgba[3]);

        int result = font->DrawText(sprite, string, count,
            &rect, flags, color);
        assert(result != 0);
        return result;
    }

    int draw_native_text(com_ptr<ID3DXFont> font, std::string const& text,
        box2i const& area, vector4i const& rgba, DWORD flags)
    {
        RECT rect;
        rect.top = min_axis(area, 1);
        rect.left = min_axis(area, 0);
        rect.bottom = max_axis(area, 1);
        rect.right = max_axis(area, 0);

        return draw_native_text(font, text, rect, rgba, flags);
    }

    //RECT to_rect(box2i const& area)
    //{
    //    RECT rect;
    //    rect.top = min_axis(area, 1);
    //    rect.left = min_axis(area, 0);
    //    rect.bottom = max_axis(area, 1);
    //    rect.right = max_axis(area, 0);
    //    return rect;
    //}
} // anonymous namespace

font::font(graphics_device& device, com_ptr<ID3DXFont> const& font)
:   device_child(device)
,   m_font(font)
{
}

font::~font()
{
}

com_ptr<ID3DXFont> const& font::native() const
{
    return m_font;
}

void font::before_destruction()
{
}

void font::before_reset()
{
    m_font->OnLostDevice();
}

void font::after_reset()
{
    m_font->OnResetDevice();
}

std::shared_ptr<font> create_font(graphics_device& device)
{
    return std::make_shared<font>(device, create_d3dx_font(device));
}

void draw_text(font const& font, std::string const& text,
    RECT& rect, vector4i const& rgba, DWORD flags)
{
    draw_native_text(font.native(), text, rect, rgba, flags);
}

void draw_text(font const& font, std::string const& text,
    box2i const& area, vector4i const& rgba, int flags)
{
    draw_native_text(font.native(), text, area, rgba,
        static_cast<UINT>(flags));
}

box2i required_area(font const& font, std::string const& text,
    box2i const& area)
{
    RECT rect = win32::rect_from(area);
    vector4i black;
    PT_DEBUG_ONLY(int height = )draw_native_text(font.native(), text, rect, black,
        DT_CALCRECT);

    assert(height <= rect.bottom - rect.top);

    return win32::box_from(rect);
}

com_ptr<ID3DXFont> create_d3dx_font(graphics_device& device,
    std::string const& name, int height)
{
    win32::log_font_container fonts = win32::enum_font(ANSI_CHARSET, name);
    assert(!fonts.empty());

    //INT height = height;
    UINT width = 0;
    UINT weight = 0;
    UINT mip_levels = D3DX_DEFAULT;
    BOOL italic = FALSE;
    DWORD char_set = ANSI_CHARSET;
    DWORD output_precision = OUT_TT_ONLY_PRECIS;
    DWORD raster_font_quality = 0;
    DWORD pitch_and_family = 0;
    LPCTSTR face_name = name.c_str();
    com_ptr<ID3DXFont> font;

    check_dx(D3DXCreateFont(device.native().get(),
        height, width, weight, mip_levels, italic, char_set,
        output_precision, raster_font_quality, pitch_and_family,
        face_name, font.receive()), "D3DXCreateFont");

    return font;
}

}} // namespace pt::d3d9
