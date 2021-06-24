#ifndef PT_WIN32_FONT_H
#define PT_WIN32_FONT_H

#include "../defs.h"
#include "../win32.h"
#include "../boost/smart_ptr.h"
#include "../std/string.h"
#include "../std/vector.h"

namespace pt { namespace win32 {

/// Container for logical fonts.
typedef std::vector<LOGFONT> log_font_container;

/// Enumerates one font for each available typeface name in \a character_set.
/// 
/// \param character_set To enumerate all available character sets, set this
///     to DEFAULT_CHARSET. To enumerate only western, set this to
///     ANSI_CHARSET. Refer Platform SDK for other character sets.
/// \return Container including one font for each available typeface name in
///     \a character_set.
/// \relatesalso win32::enum_font
log_font_container enum_font_families(long character_set);

/// Enumerates all fonts with the specified \a name in \a character_set.
/// \return Container including all fonts with \a name in \a character_set.
/// \relatesalso win32::enum_font_families
log_font_container enum_font(long character_set, std::string const& name);

void test_font();

}} // namespace pt::win32

#endif
