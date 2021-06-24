#pragma once
#ifndef PT_D3D9_SHADERS_SHADER_UTILS_H
#define PT_D3D9_SHADERS_SHADER_UTILS_H

namespace pt {
    class config_node;
    class path;
    class string_wrapper;
}

namespace pt { namespace d3d9 { namespace shaders {

string_wrapper read_hlsl_from_file(path file_path);
string_wrapper parse_hlsl(config_node const& config);

}}} // namespace pt::d3d9::shaders

#endif
