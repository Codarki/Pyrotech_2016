#pragma once
#ifndef PT_INPUT_STREAM_UTILITY_H
#define PT_INPUT_STREAM_UTILITY_H

#include "pt/std/string.h"

namespace pt {

class path;

std::string read_text_file(path const& path);

} // namespace pt

#endif
