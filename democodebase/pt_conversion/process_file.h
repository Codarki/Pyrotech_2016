#ifndef PT_CONVERSION_PROCESS_FILE_H
#define PT_CONVERSION_PROCESS_FILE_H

#include "pt/path.h"

namespace pt { namespace conversion {

/// Converts file \a src into \a dst.
void process_file(
    path const& src,
    path const& dst,
    path const& dst_root);

}} // namespace pt::conversion

#endif
