#include "point_light_shader.h"
#include "shader_utils.h"
#include "pt/config_node.h"
#include "pt/path.h"
#include "pt/string_wrapper.h"

namespace pt { namespace d3d9 { namespace shaders {

namespace {
//    std::string read_point_light_hlsl()
//    {
//        pt::path shader_path("data/shaders/light/hlsl/point_light.hlsl");
//        return read_hlsl_from_file(shader_path).string();
//    }
    std::string read_dummy_light_hlsl()
    {
        pt::path shader_path("data/shaders/light/hlsl/dummy_light.hlsl");
        return read_hlsl_from_file(shader_path).string();
    }
//    std::string read_frustum_light_hlsl()
//    {
//        pt::path shader_path("data/shaders/light/hlsl/frustum_light.hlsl");
//        return read_hlsl_from_file(shader_path).string();
//    }
//    std::string read_shadow_mapped_frustum_light_hlsl()
//    {
//        pt::path shader_path("data/shaders/light/hlsl/shadow_mapped_frustum_light.hlsl");
//        return read_hlsl_from_file(shader_path).string();
//    }

} // anonymous namespace

// basic_light_shader

basic_light_shader::basic_light_shader(
    std::string const& name,
    std::string const& pixel_hlsl,
    config_node parameters,
    config_node sampler_states)
: m_name(name)
, m_pixel_hlsl(pixel_hlsl)
, m_parameters(parameters)
, m_sampler_states(sampler_states)
{
}

std::string const& basic_light_shader::name() const
{
    return m_name;
}

std::string const& basic_light_shader::pixel_shader() const
{
    return m_pixel_hlsl;
}

config_node const& basic_light_shader::parameters() const
{
    return m_parameters;
}

config_node const& basic_light_shader::sampler_states() const
{
    return m_sampler_states;
}

//// point_light_shader
//
//point_light_shader::point_light_shader()
//: m_pixel_hlsl(read_point_light_hlsl())
//{
//}
//
//std::string const& point_light_shader::pixel_shader() const
//{
//    return m_pixel_hlsl;
//}

// dummy_light_shader

dummy_light_shader::dummy_light_shader()
: m_pixel_hlsl(read_dummy_light_hlsl())
, m_name("dummy_light_shader")
{
}

std::string const& dummy_light_shader::name() const
{
    return m_name;
}

std::string const& dummy_light_shader::pixel_shader() const
{
    return m_pixel_hlsl;
}

config_node const& dummy_light_shader::parameters() const
{
    return m_empty_config;
}

config_node const& dummy_light_shader::sampler_states() const
{
    return m_empty_config;
}


//// frustum_light_shader
//
//frustum_light_shader::frustum_light_shader()
//: m_pixel_hlsl(read_frustum_light_hlsl())
//{
//}
//
//std::string const& frustum_light_shader::pixel_shader() const
//{
//    return m_pixel_hlsl;
//}
//
//// shadow_mapped_frustum_light_shader
//
//shadow_mapped_frustum_light_shader::shadow_mapped_frustum_light_shader()
//: m_pixel_hlsl(read_shadow_mapped_frustum_light_hlsl())
//{
//}
//
//std::string const& shadow_mapped_frustum_light_shader::pixel_shader() const
//{
//    return m_pixel_hlsl;
//}

// Free functions

std::unique_ptr<basic_light_shader> load_light_shader(pt::path const& path)
{
    config_node main_config(path);
    config_node light_shader_config = *main_config.child("light_shader");

    auto pixel_shader = parse_hlsl(*light_shader_config.child("pixel_shader")).string();
    auto parameters = *light_shader_config.child("parameters");
    auto sampler_states = *light_shader_config.child("sampler_states");

    return std::make_unique<basic_light_shader>(path.string(), pixel_shader, parameters, sampler_states);
}

}}} // namespace pt::d3d9::shaders
