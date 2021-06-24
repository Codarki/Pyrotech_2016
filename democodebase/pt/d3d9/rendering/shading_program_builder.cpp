#include "shading_program_builder.h"
#include "shading_program.h"
#include "pt/d3d9/shaders/light_shader.h"
#include "pt/d3d9/shaders/surface_shader.h"
#include "pt/d3d9/error.h"
#include "pt/d3d9/pixel_shader.h"
#include "pt/d3d9/vertex_shader.h"
#include "pt/d3d9/shader_convenience.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/exceptions/string_context.h"

#include "renderer.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    std::shared_ptr<pixel_shader> create_pixel_shader(
        graphics_device& device,
        shaders::surface_shader const& surface_shader,
        shaders::light_shader const& light_shader)
    {
        try
        {
            auto hlsl = light_shader.pixel_shader()
                + surface_shader.pixel_shader();

            return compile_new_pixel_shader(device, hlsl,
                surface_shader.pixel_shader_name(), "ps_3_0");
        }
        catch (compilation_error& e)
        {
            e.attach_context(new exceptions::string_context(format(
                "pixel shader \"%1%\"") % surface_shader.name()));
            throw;
        }
    }

    //std::shared_ptr<pixel_shader> create_alpha_tested_depth_pixel_shader(
    //    graphics_device& device,
    //    shaders::surface_shader const& surface_shader)
    //{
    //    try
    //    {
    //        auto hlsl = surface_shader.pixel_shader();

    //        return compile_new_pixel_shader(device, hlsl,
    //            surface_shader.pixel_shader_name(), "ps_3_0");
    //    }
    //    catch (compilation_error& e)
    //    {
    //        e.attach_context(new exceptions::string_context(format(
    //            "pixel shader \"%1%\"") % surface_shader.name()));
    //        throw;
    //    }
    //}

    std::shared_ptr<vertex_shader> create_vertex_shader(
        graphics_device& device,
        shaders::surface_shader const& surface_shader)
    {
        try
        {
            return compile_new_vertex_shader(device,
                surface_shader.vertex_shader(),
                surface_shader.vertex_shader_name(), "vs_3_0");
        }
        catch (compilation_error& e)
        {
            e.attach_context(new exceptions::string_context(format(
                "vertex shader \"%1%\"") % surface_shader.name()));
            throw;
        }
    }
} // anonymous

shared_shading_program create_shared_shading_program(
    graphics_device& device,
    shaders::surface_shader const& surface_shader,
    shaders::light_shader const& light_shader)
{
    log(format("Creating shading program: '%1%'") % surface_shader.name());

    auto pixel_shader = create_pixel_shader(device, surface_shader, light_shader);
    auto vertex_shader = create_vertex_shader(device, surface_shader);

    return std::make_shared<shading_program>(device, pixel_shader, vertex_shader);
}

shared_shading_program create_shared_depth_shading_program(
    graphics_device& device,
    shaders::surface_shader const& surface_shader)
{
    log(format("Creating depth shading program: '%1%'") % surface_shader.name());

    auto vertex_shader = create_vertex_shader(device, surface_shader);
    std::shared_ptr<pixel_shader> pixel_shader;
    //if (surface_shader.is_pixel_required_for_depth())
    {
        shaders::light_shader const& light_shader = d3d9::rendering::global_dummy_light_shader();

        pixel_shader = create_pixel_shader(device, surface_shader, light_shader);
    }

    return std::make_shared<shading_program>(device, pixel_shader, vertex_shader);
}

}}} // namespace pt::d3d9::rendering
