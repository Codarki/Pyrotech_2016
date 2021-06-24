#include "material_description.h"
#include "pt/log.h"
#include "pt/format.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

// shader_description

shader_description::shader_description(pt::path const& shader_path,
    config_node const& parameters, pt::path const& pre_shader_path)
: m_shader_path(shader_path)
, m_parameters(parameters)
, m_pre_shader_path(pre_shader_path)
{
}

pt::path const& shader_description::shader_path() const
{
    return m_shader_path;
}

pt::path const& shader_description::pre_shader_path() const
{
    return m_pre_shader_path;
}

config_node const& shader_description::parameters() const
{
    return m_parameters;
}

// material_description

material_description::material_description(std::string const& name,
    std::string const& p3d_name, std::vector<shader_description> shader_description)
: m_name(name)
, m_p3d_name(p3d_name)
, m_shader_description(shader_description)
{
}

std::string const& material_description::name() const
{
    return m_name;
}

std::string const& material_description::p3d_name() const
{
    return m_p3d_name;
}

std::vector<shader_description> const& material_description::shader_descriptions() const
{
    return m_shader_description;
}

// material_description_builder

std::unique_ptr<material_description> material_description_builder::build_unique(
    config_node const& material) const
{
    //return std::unique_ptr<material_description>(build_auto(material));

    assert(material.key() == "material");

    std::string const& name = material.child("name")->value();
    std::string const p3d_name
        = optional_child_value_text(material, "material_p3d", "");

    std::vector<shader_description> shader_descriptions;
    for (auto const& node : material)
    {
        if (node.key() == "shader")
        {
            shader_descriptions.push_back(build_shader_description(node, name));
        }
    }

    //std::string const p3d_name = material.child("material_p3d")->value();
    return std::auto_ptr<material_description>(
        new material_description(name, p3d_name, shader_descriptions));
}

shader_description material_description_builder::build_shader_description(
    config_node const& config, std::string material_name) const
{
    path shader_path = path(config.child("file")->value());

    std::string preshader_path = optional_child_value_text(config, "preshader", "");

    if (!preshader_path.empty())
        log(format("preshader %1% for material %2% found!") % preshader_path % material_name);

    config_node parameters = *config.child("parameters");

    return shader_description(shader_path, parameters, preshader_path);
}

// Free functions

std::unique_ptr<material_description> build_unique_light_material_description(
    config_node const& material)
{
    assert(material.key() == "light_shader_instance");

    std::string const& name = material.child("name")->value();
    path shader_path = path(material.child("shader")->value());

    config_node parameters = *material.child("parameters");

    std::vector<shader_description> shader_descriptions;
    pt::path preshader_path;
    shader_descriptions.push_back(shader_description(shader_path, parameters, preshader_path));

    return std::auto_ptr<material_description>(
        new material_description(name, "", shader_descriptions));
}

}}}} // namespace pt::d3d9::rendering::source
