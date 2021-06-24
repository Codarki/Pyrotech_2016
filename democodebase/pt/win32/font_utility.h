#ifndef PT_WIN32_FONT_UTILITY_H
#define PT_WIN32_FONT_UTILITY_H

#include "../defs.h"
#include "../win32.h"
#include "../boost/smart_ptr.h"
#include "../std/string.h"

namespace pt { namespace win32 {

/// \return \a charset formatted for human readable text.
std::string charset_as_human_readable_string(int charset);
/// \return \a charset as it is defined, all uppercases. eg. ANSI_CHARSET
std::string charset_as_string(int charset);

/// Launches a dialog box for choosing a font.
HFONT choose_font(HWND handle, DWORD flags);

}} // namespace pt::win32

#endif
