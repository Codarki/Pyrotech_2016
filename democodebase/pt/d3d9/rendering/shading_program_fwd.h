#pragma once
#ifndef PT_D3D9_RENDERING_SHADING_PROGRAM_FWD_H
#define PT_D3D9_RENDERING_SHADING_PROGRAM_FWD_H

#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class shading_program;
using shared_shading_program = std::shared_ptr<shading_program>;

}}} // namespace pt::d3d9::rendering

#endif
