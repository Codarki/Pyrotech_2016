#ifndef PT_CONVERSION_CONVERSION_H
#define PT_CONVERSION_CONVERSION_H

#include "pt/std/string.h"
#include "pt/path.h"

namespace pt { namespace conversion {

/// Recursively convert "./src-dir/" into "./data/" directory.
//void convert();

/// Recursively convert "./source_dir/src-dir/" into "./data/" directory.
void convert(std::string const& source_dir, std::string const& target_dir);

struct conversion_settings
{
    path source_directory;
    path intermediate_directory;
    path target_directory;
};

void convert(conversion_settings const& settings);

}} // namespace pt::conversion

#endif
