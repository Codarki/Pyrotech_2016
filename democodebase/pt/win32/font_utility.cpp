#include "font_utility.h"
#include <commdlg.h>

namespace pt { namespace win32 {

std::string charset_as_human_readable_string(int charset)
{
    switch(charset)
    {
    case ANSI_CHARSET:
        return "Western";
    case DEFAULT_CHARSET:
        return "All";
    case SYMBOL_CHARSET:
        return "Symbol";
    case SHIFTJIS_CHARSET:
        return "ShiftJIS";
    case HANGUL_CHARSET:
        return "Hangul";
    case GB2312_CHARSET:
        return "GB2312";
    case CHINESEBIG5_CHARSET:
        return "Chinese BIG5";
    case OEM_CHARSET:
        return "OEM";
    case JOHAB_CHARSET:
        return "Johab";
    case HEBREW_CHARSET:
        return "Hebrew";
    case ARABIC_CHARSET:
        return "Arabic";
    case GREEK_CHARSET:
        return "Greek";
    case TURKISH_CHARSET:
        return "Turkish";
    case VIETNAMESE_CHARSET:
        return "Vietnamese";
    case THAI_CHARSET:
        return "Thai";
    case EASTEUROPE_CHARSET:
        return "Eastern European";
    case RUSSIAN_CHARSET:
        return "Russian";
    case MAC_CHARSET:
        return "Macintosh";
    case BALTIC_CHARSET:
        return "Baltic";
    }
    return "Unknown charset";
}

std::string charset_as_string(int charset)
{
    switch(charset)
    {
    case ANSI_CHARSET:
        return "ANSI_CHARSET";
    case DEFAULT_CHARSET:
        return "DEFAULT_CHARSET";
    case SYMBOL_CHARSET:
        return "SYMBOL_CHARSET";
    case SHIFTJIS_CHARSET:
        return "SHIFTJIS_CHARSET";
    case HANGUL_CHARSET:
        return "HANGUL_CHARSET";
    case GB2312_CHARSET:
        return "GB2312_CHARSET";
    case CHINESEBIG5_CHARSET:
        return "CHINESEBIG5_CHARSET";
    case OEM_CHARSET:
        return "OEM_CHARSET";
    case JOHAB_CHARSET:
        return "JOHAB_CHARSET";
    case HEBREW_CHARSET:
        return "HEBREW_CHARSET";
    case ARABIC_CHARSET:
        return "ARABIC_CHARSET";
    case GREEK_CHARSET:
        return "GREEK_CHARSET";
    case TURKISH_CHARSET:
        return "TURKISH_CHARSET";
    case VIETNAMESE_CHARSET:
        return "VIETNAMESE_CHARSET";
    case THAI_CHARSET:
        return "THAI_CHARSET";
    case EASTEUROPE_CHARSET:
        return "EASTEUROPE_CHARSET";
    case RUSSIAN_CHARSET:
        return "RUSSIAN_CHARSET";
    case MAC_CHARSET:
        return "MAC_CHARSET";
    case BALTIC_CHARSET:
        return "BALTIC_CHARSET";
    }
    return "Unknown charset";
}

HFONT choose_font(HWND handle, DWORD flags) 
{ 
    CHOOSEFONT cf;
    LOGFONT lf;

    flags |= CF_SCREENFONTS;
    //flags |= CF_EFFECTS;
    //flags |= CF_FORCEFONTEXIST;
 
    // Initialize members of the CHOOSEFONT structure.
    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = handle;
    cf.hDC = static_cast<HDC>(0);
    cf.lpLogFont = &lf; 
    cf.iPointSize = 0; 
    cf.Flags = flags; 
    cf.rgbColors = RGB(0,0,0); 
    cf.lCustData = 0L; 
    cf.lpfnHook = (LPCFHOOKPROC)NULL; 
    cf.lpTemplateName = (LPSTR)NULL; 
    cf.hInstance = (HINSTANCE) NULL; 
    cf.lpszStyle = (LPSTR)NULL; 
    cf.nFontType = SCREEN_FONTTYPE; 
    cf.nSizeMin = 0; 
    cf.nSizeMax = 0; 
 
    // Display the CHOOSEFONT common-dialog box. 
    ChooseFont(&cf); 
 
    // Create a logical font based on the user's 
    // selection and return a handle identifying 
    // that font. 
    HFONT hfont = CreateFontIndirect(cf.lpLogFont); 
    return hfont;
}

}} // namespace pt::win32
