#include "shader_convenience.h"
#include "shader_builder.h"
#include "error.h"
#include "pixel_shader.h"
#include "vertex_shader.h"
#include "pt/log.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 {

DWORD const* compiled_shader::data()
{
    assert(shader);
    return reinterpret_cast<DWORD const*>(shader->GetBufferPointer());
}

compiled_shader compile_shader(std::string const& hlsl,
    std::string const& function_name, std::string const& profile)
{
    D3DXMACRO const* defines = 0;
    LPD3DXINCLUDE include = 0;
    DWORD flags = D3DXSHADER_OPTIMIZATION_LEVEL1 | D3DXSHADER_PREFER_FLOW_CONTROL;//D3DXSHADER_USE_LEGACY_D3DX9_31_DLL | D3DXSHADER_PREFER_FLOW_CONTROL;//D3DXSHADER_OPTIMIZATION_LEVEL0;

    compiled_shader result;

    LPD3DXBUFFER error_messages;

    HRESULT hr = D3DXCompileShader(
        hlsl.c_str(),
        static_cast<UINT>(hlsl.length()),
        defines,
        include,
        function_name.c_str(),
        profile.c_str(),
        flags,
        result.shader.receive(),
        &error_messages,
        result.constants.receive());

    if (error_messages)
    {
        char const* ptr
            = reinterpret_cast<char const*>(error_messages->GetBufferPointer());
        //log(ptr);
        throw compilation_error(ptr);
    }

    check_dx(hr, "D3DXCompileShader");

    return result;
}

pixel_shader compile_pixel_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile)
{
    compiled_shader result
        = compile_shader(hlsl, function_name, profile);

    shader_builder builder(device);

    return pixel_shader(device, builder.build_pixel_shader(result.data()), result.constants);
}

vertex_shader compile_vertex_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile)
{
    compiled_shader result
        = compile_shader(hlsl, function_name, profile);

    shader_builder builder(device);

    return vertex_shader(device, builder.build_vertex_shader(result.data()), result.constants);
}

std::shared_ptr<pixel_shader> compile_new_pixel_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile)
{
    compiled_shader result
        = compile_shader(hlsl, function_name, profile);

    shader_builder builder(device);

    return std::make_shared<pixel_shader>(
        device, builder.build_pixel_shader(result.data()), result.constants);
}

std::shared_ptr<vertex_shader> compile_new_vertex_shader(graphics_device& device,
    std::string const& hlsl, std::string const& function_name,
    std::string const& profile)
{
    compiled_shader result
        = compile_shader(hlsl, function_name, profile);

    shader_builder builder(device);

    return std::make_shared<vertex_shader>(
        device, builder.build_vertex_shader(result.data()), result.constants);
}

}} // namespace pt::d3d9
