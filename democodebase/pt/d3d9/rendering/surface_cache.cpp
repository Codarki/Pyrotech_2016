#include "surface_cache.h"
#include "surface.h"
#include "surface_variables.h"
#include "source/material.h"
#include "pt/parameter_node.h"
#include "pt/animatable_value_base.h"
#include "pt/string_wrapper.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
	std::shared_ptr<surface> build_shared_surface(
        std::vector<source::material_shader> const& material_shaders)
        //surface_shader_parameters const& parameters,
        //surface_shader_parameters const& pre_shader_parameters)
    {
        std::vector<surface_variables_instance> instances;
        for (auto const& material_shader : material_shaders)
        {
            auto variables = std::make_shared<surface_variables>(material_shader.shader_parameters());
            auto pre_shader_variables = std::make_shared<surface_variables>(material_shader.pre_shader_parameters());

            auto instance = surface_variables_instance(variables, pre_shader_variables);
            instances.push_back(instance);
        }

        return std::make_shared<surface>(instances);
    }
} // anonymous namespace

surface_cache::surface_cache()
{
}

std::shared_ptr<surface> surface_cache::find_or_create(key_type key)
{
    surface_map::const_iterator i = m_surfaces.find(key);
    if (i != m_surfaces.end())
    {
        return i->second;
    }

    std::shared_ptr<surface> result = build_shared_surface(
        key->shaders());
        //key->shaders()[0].shader_parameters(), key->shaders()[0].pre_shader_parameters());

    m_surfaces.insert(surface_map::value_type(key, result));

    return result;
}

std::string surface_cache::parameters_as_readable_text() const
{
    return "not implemented";
    //std::string result;
    //result.reserve(200);

    //for (auto it = m_surfaces.begin(); it != m_surfaces.end(); ++it)
    //{
    //    source::material const& material = *it->first;
    //    surface const& surface = *it->second;

    //    result += material.name() + "\r\n";
    //    result += hierarchy_as_readable_text(surface.parameters(), 2).string() + "\r\n";
    //}

    //return result;
}

}}} // namespace pt::d3d9::rendering
