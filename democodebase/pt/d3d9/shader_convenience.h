#ifndef PT_D3D9_SHADER_CONVENIENCE_H
#define PT_D3D9_SHADER_CONVENIENCE_H

#include "native.h"
#include "graphics_device_fwd.h"
#include "pt/com_ptr.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {

class pixel_shader;
class vertex_shader;

// Convenience direct3d shader functions.

// temporary struct. todo refactor away.
struct compiled_shader
{
    com_ptr<ID3DXBuffer> shader;
    com_ptr<ID3DXConstantTable> constants;

    DWORD const* data();
};

compiled_shader compile_shader(std::string const& hlsl,
    std::string const& function_name, std::string const& profile);

pixel_shader compile_pixel_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile);

vertex_shader compile_vertex_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile);

std::shared_ptr<pixel_shader> compile_new_pixel_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile);

std::shared_ptr<vertex_shader> compile_new_vertex_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile);

}} // namespace pt::d3d9

#endif
