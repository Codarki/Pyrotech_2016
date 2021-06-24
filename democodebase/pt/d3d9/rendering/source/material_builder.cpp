#include "material_builder.h"
#include "light_material.h"
#include "material.h"
#include "material_description.h"
#include "pt/d3d9/rendering/light_shader_parameter_builder.h"
#include "pt/d3d9/rendering/surface_shader_parameter_builder.h"
#include "pt/d3d9/shaders/light_shader.h"
#include "pt/d3d9/shaders/light_shader_collection.h"
#include "pt/d3d9/shaders/point_light_shader.h"
#include "pt/d3d9/shaders/surface_shader.h"
#include "pt/d3d9/shaders/surface_shader_collection.h"
#include "pt/log.h"
#include "pt/scoped_assignment.h"
#include "pt/format.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

material_builder::material_builder()
{
}

material_builder::material_builder(
    std::unique_ptr<material_description> description,
    std::vector<std::shared_ptr<shaders::surface_shader>> shaders,
    std::vector<std::shared_ptr<shaders::surface_shader>> pre_shaders)
: m_description(move(description))
, m_shaders(shaders)
, m_pre_shaders(pre_shaders)
{
    ////assert(m_description->shader_descriptions().size() == pre_shaders.size());
    //if (m_description->shader_descriptions().size() != pre_shaders.size())
    //{
    //    int asd = 0;
    //    asd;
    //}
}

material_builder::~material_builder()
{
}

std::string const& material_builder::name() const
{
    assert(m_description);
    return m_description->name();
}

std::string const& material_builder::p3d_material_name() const
{
    assert(m_description);
    return m_description->p3d_name();
}

void material_builder::build(parameter_collection const& parameter_overrides,
    parameter_collection const& shared_parameters,
    graphics_device& device)
{
    assert(m_description);

    // We support building a material single time.
    if (m_material)
        return;

    assert(m_shaders.size() > 0);

    std::vector<material_shader> shaders;
    size_t i_preshader = 0;
    for (size_t i = 0; i < m_description->shader_descriptions().size(); ++i)
    {
        auto const& shader_description = m_description->shader_descriptions()[i];

        log(format("Building material '%1%' using shader '%2%'")
            % m_description->name() % m_shaders[i]->name());

        surface_shader_parameter_builder parameter_builder(m_shaders[i], shared_parameters, device);
        surface_shader_parameters shader_parameters = parameter_builder.build(shader_description, parameter_overrides);

        parameter_collection empty_parameter_collection;
        config_node empty_config_node;
        rendering::shader_parameters shader_parameters2(empty_parameter_collection, empty_config_node);
        surface_shader_parameters pre_shader_parameters(nullptr, shader_parameters2);

        if (!shader_description.pre_shader_path().empty())
        {
            surface_shader_parameter_builder parameter_builder_preshader(m_pre_shaders[i_preshader],
                shared_parameters, device);

            pre_shader_parameters = parameter_builder_preshader.build(shader_description, parameter_overrides);
            ++i_preshader;
        }

        shaders.push_back(material_shader(shader_parameters, pre_shader_parameters));
    }
    
    m_material.reset(new material(m_description->name(), shaders));
}

std::shared_ptr<material> material_builder::built_material()
{
    return m_material;
}

// light_material_builder

light_material_builder::light_material_builder()
{
}

light_material_builder::light_material_builder(
    std::unique_ptr<material_description> description,
    std::shared_ptr<shaders::light_shader> shader)
: m_description(move(description))
, m_shader(shader)
{
}

light_material_builder::~light_material_builder()
{
}

std::string const& light_material_builder::name() const
{
    return m_description->name();
}

void light_material_builder::build(
    parameter_collection const& parameter_overrides,
    parameter_collection const& shared_parameters,
    graphics_device& device)
{
    assert(m_description);

    // We support building a material single time.
    if (m_light_material)
        return;

    assert(m_shader);

    log(format("Building light material '%1%' using shader '%2%'")
        % m_description->name() % m_shader->name());

    // TODO: Multiple shaders
    assert(m_description->shader_descriptions().size() == 1 && "TODO: Multiple shaders");

    light_shader_parameter_builder parameter_builder(m_shader, shared_parameters, device);
    light_shader_parameters shader_parameters = parameter_builder.build(m_description->shader_descriptions()[0], parameter_overrides);

    m_light_material.reset(new light_material(m_description->name(), shader_parameters));
}

std::shared_ptr<light_material> light_material_builder::built_material()
{
    return m_light_material;
}

// Free functions

material_builder create_material_builder(config_node const& config,
    shaders::surface_shader_collection& shaders)
{
    auto description = material_description_builder().build_unique(config);

    std::vector<std::shared_ptr<shaders::surface_shader>> surface_shaders;
    std::vector<std::shared_ptr<shaders::surface_shader>> pre_shaders;

    for (auto const& shader_description : description->shader_descriptions())
    {
        auto shader = shaders.find(shader_description.shader_path());
        if (!shader)
        {
            shader = shaders::load_surface_shader(shader_description.shader_path());
            shaders.add(shader_description.shader_path(), shader);
        }

        std::shared_ptr<shaders::surface_shader> pre_shader;
        if (!shader_description.pre_shader_path().empty())
        {
            pre_shader = shaders.find(shader_description.pre_shader_path());
            if (!pre_shader)
            {
                pre_shader = shaders::load_surface_shader(shader_description.pre_shader_path());
                shaders.add(shader_description.pre_shader_path(), pre_shader);
            }
            pre_shaders.push_back(pre_shader);
        }

        surface_shaders.push_back(shader);
    }

    return material_builder(move(description), surface_shaders, pre_shaders);
}

light_material_builder create_light_material_builder(
    config_node const& config,
    shaders::light_shader_collection& shaders)
{
    auto description = build_unique_light_material_description(config);

    auto shader = shaders.find(description->shader_descriptions()[0].shader_path());
    if (!shader)
    {
        shader = shaders::load_light_shader(description->shader_descriptions()[0].shader_path());
        shaders.add(description->shader_descriptions()[0].shader_path(), shader);
    }

    //assert(false);
    return light_material_builder(move(description), shader);
}

}}}} // namespace pt::d3d9::rendering::source
