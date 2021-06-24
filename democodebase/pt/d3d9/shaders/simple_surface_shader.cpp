#include "simple_surface_shader.h"
#include "shader_utils.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/config_node.h"
#include "pt/file_input_stream.h"
#include "pt/data_stream_content.h"
#include "pt/format.h"
#include "pt/log.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace shaders {

void simple_surface_shader::vertex_shader_desc::read(config_node const& config)
{
    assert(config.key() == "vertex_shader");
    config.parse_child("stream", stream);
    function_name = "main_vs";

    hlsl = parse_hlsl(config).string();
}

void simple_surface_shader::pixel_shader_desc::read(config_node const& config)
{
    assert(config.key() == "pixel_shader");
    function_name = "main_ps";

    hlsl = parse_hlsl(config).string();
}

// simple_surface_shader

simple_surface_shader::simple_surface_shader(path const& path)
: m_path(path)
, m_name(path.string())
, m_is_additive_blended(false)
, m_is_transparent(false)
, m_is_alpha_tested(false)
, m_is_pixel_required_for_depth(false)
, m_d3d_cullmode(D3DCULL_CCW)
{
    config_node config(path);

    std::string cull_mode_text;

    config.parse_optional_child("additive_blend", m_is_additive_blended);
    config.parse_optional_child("is_transparent", m_is_transparent);
    config.parse_optional_child("is_alpha_tested", m_is_alpha_tested);
    config.parse_optional_child("is_pixel_required_for_depth", m_is_pixel_required_for_depth);
    config.parse_optional_child("cull_mode", cull_mode_text, std::string("CCW"));

    if (cull_mode_text == "NONE")
    {
        m_d3d_cullmode = D3DCULL_NONE;
    }
    else if (cull_mode_text == "CW")
    {
        m_d3d_cullmode = D3DCULL_CW;
    }
    else if (cull_mode_text == "CCW")
    {
        m_d3d_cullmode = D3DCULL_CCW;
    }

    m_streams.read(*config.child("streams"));
    m_vertex_shader.read(*config.child("vertex_shader"));
    m_pixel_shader.read(*config.child("pixel_shader"));

    m_parameters = *config.child("parameters");
    m_sampler_states = *config.child("sampler_states");
}

std::string const& simple_surface_shader::pixel_shader() const
{
    return m_pixel_shader.hlsl;
}

std::string const& simple_surface_shader::pixel_shader_name() const
{
    return m_pixel_shader.function_name;
}

std::string const& simple_surface_shader::vertex_shader() const
{
    return m_vertex_shader.hlsl;
}

std::string const& simple_surface_shader::vertex_shader_name() const
{
    return m_vertex_shader.function_name;
}

std::string const& simple_surface_shader::name() const
{
    return m_name;
}

stream_collection const& simple_surface_shader::streams() const
{
    return m_streams;
}

config_node const& simple_surface_shader::parameters() const
{
    return m_parameters;
}

config_node const& simple_surface_shader::sampler_states() const
{
    return m_sampler_states;
}

bool simple_surface_shader::is_additive_blended() const
{
    return m_is_additive_blended;
}

bool simple_surface_shader::is_transparent() const
{
    return m_is_transparent;
}

bool simple_surface_shader::is_alpha_tested() const
{
    return m_is_alpha_tested;
}

bool simple_surface_shader::is_pixel_required_for_depth() const
{
    return m_is_pixel_required_for_depth || m_is_alpha_tested;
}

int simple_surface_shader::d3d_cullmode() const
{
    return m_d3d_cullmode;
}

}}} // namespace pt::d3d9::shaders
