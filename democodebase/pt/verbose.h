#ifndef PT_VERBOSE_H
#define PT_VERBOSE_H

#include "std/string.h"

namespace pt {

/// Enable or disable verbose.
void enable_verbose(bool enable);

/// \return \c true if verbose enabled
bool verbose_enabled();

/// Prints \a text into VERBOSE.txt
void verbose(const char* text);
/// Prints \a text into VERBOSE.txt
void verbose(std::string const& text);

} // namespace pt

#endif
