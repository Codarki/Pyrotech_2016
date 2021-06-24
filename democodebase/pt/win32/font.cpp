#include "font.h"
#include "font_utility.h"
#include "pt/log.h"
#include "pt/format.h"

#include <wingdi.h>

namespace pt { namespace win32 {

/// \param log_font Pointer to logical-font data.
/// \param metric Pointer to physical-font data.
/// \param font_type Type of font.
/// \param lparam log_font_container for results
    
//int CALLBACK font_family_enum_proc(LOGFONTA const* log_font,
//    NEWTEXTMETRIC const* metric, DWORD font_type, LPARAM lparam)
int CALLBACK font_family_enum_proc(LOGFONTA const* log_font,
    TEXTMETRICA const* metric, DWORD font_type, LPARAM lparam)
{
    assert(log_font && metric);
    std::string font_text("Font: ");
    font_text += reinterpret_cast<char const*>(&log_font->lfFaceName[0]);

    assert(reinterpret_cast<log_font_container*>(lparam));
    log_font_container& result = *(reinterpret_cast<log_font_container*>(
        lparam));

    result.push_back(*log_font);

    // An application can also check bits 1 and 2 in the tmPitchAndFamily
    // member of the NEWTEXTMETRIC structure to identify a TrueType font.
    // If bit 1 is 0 and bit 2 is 1, the font is a TrueType font. 
    bool truetype = !(metric->tmPitchAndFamily & 1)
        && (metric->tmPitchAndFamily & 2);

    bool is_italic = metric->tmItalic != 0;
    bool is_underlined = metric->tmUnderlined  != 0;
    bool is_strikeout = metric->tmStruckOut  != 0;

    // besides having their own bits set, TrueType and PostScript fonts set the
    // TMPF_VECTOR bit

    //bool is_fixed_pitch = !(metric->tmPitchAndFamily & TMPF_FIXED_PITCH);
    //bool is_vector = (metric->tmPitchAndFamily & TMPF_VECTOR) != 0;
    //bool is_truetype = (metric->tmPitchAndFamily & TMPF_TRUETYPE) != 0;
    //bool is_device = (metric->tmPitchAndFamily & TMPF_DEVICE)  != 0;

    font_text += "\t(";

    if (!(font_type & RASTER_FONTTYPE) && !(font_type & TRUETYPE_FONTTYPE))
    {
        font_text += "Vector font";

        assert(metric->tmCharSet == OEM_CHARSET);
    }
    if (font_type & DEVICE_FONTTYPE)
    {
        font_text += "Device font";
    }
    else if (font_type & RASTER_FONTTYPE)
    {
        font_text += "Raster font";
    }
    else if (truetype || font_type & TRUETYPE_FONTTYPE)
    {
        if (!(truetype && (font_type & TRUETYPE_FONTTYPE)))
            font_text += "Truetype font (mixed flags)";
        else
            font_text += "Truetype font";
    }

    font_text += (format(", height %1%") % metric->tmHeight).move_string();

    std::string charset = charset_as_human_readable_string(metric->tmCharSet);
    if (!charset.empty())
        font_text += ", " + charset;

    if (is_italic)
        font_text += ", italic";
    if (is_underlined)
        font_text += ", underlined";
    if (is_strikeout)
        font_text += ", strikeout";

    font_text += ")";

    log(font_text);

    metric;

    bool continue_enum = true;
    return continue_enum ? 1 : 0;
}

log_font_container enum_font_families(long character_set)
{
    return enum_font(character_set, "");
}

log_font_container enum_font(long character_set, std::string const& name)
{
    log_font_container result;
    log("Enumerating fonts...");
    logging::scoped_indent ident;

    HDC hdc = GetDC(0);
    LOGFONT logfont;
    LPARAM lparam = reinterpret_cast<LPARAM>(&result);
    DWORD flags = 0;

    logfont.lfCharSet = static_cast<BYTE>(character_set);

    if (name.length() > 0)
    {
        assert(name.length() < 32);
        std::copy(name.begin(), name.end(), &logfont.lfFaceName[0]);
        logfont.lfFaceName[name.length()] = 0;
    }
    else
    {
        logfont.lfFaceName[0] = 0;
    }

    logfont.lfPitchAndFamily = 0;

    int finished = EnumFontFamiliesEx(hdc, &logfont, font_family_enum_proc,
        lparam, flags);

    if (finished == 1)
        log("All fonts enumerated.");
    else
        log("Font enumeration aborted.");

    return result;
}

/*int enum_font_families(long character_set)
{
    log("Enumerating fonts...");
    logging::scoped_indent ident;

    HDC hdc = GetDC(0);
    LOGFONT logfont;
    LPARAM lparam = 0;
    DWORD flags = 0;

    logfont.lfCharSet = static_cast<BYTE>(character_set);
    logfont.lfFaceName[0] = 0;
    logfont.lfPitchAndFamily = 0;

    int result = EnumFontFamiliesEx(hdc, &logfont, font_family_enum_proc,
        lparam, flags);
    return result;
}
*/
static int CALLBACK font_enum_proc(LOGFONT const*, TEXTMETRIC const*, DWORD,
    LPARAM lParam)
{
	if (lParam != NULL)
	{
		*(BOOL*)lParam = TRUE;
	}
	return 0;
}

void test_font()
{
    char* face = "Tahoma";
	BOOL installed;
	HDC hdc;
	LOGFONT lf;

	memset(&lf, 0, sizeof(lf));

	lstrcpy(lf.lfFaceName, face);
	lf.lfCharSet = DEFAULT_CHARSET;

	installed = FALSE;
	hdc = ::GetDC(NULL);
	if (hdc != NULL)
	{
		::EnumFontFamiliesEx(hdc, &lf, font_enum_proc, (LPARAM)&installed, 0);
		::ReleaseDC(NULL, hdc);
	}

	//return installed;
}

}} // namespace pt::win32
