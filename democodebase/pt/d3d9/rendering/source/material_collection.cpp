#include "material_collection.h"
#include "material.h"
#include "pt/exceptions/path_file_context.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/log.h"
#include "pt/parameter_node_utility.h"
#include "pt/format.h"
#include "pt/path.h"
#include "pt/boost/path.h"

#include "pt/directory_iterator.h"
#include "pt/boost/filesystem/convenience.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

// material_collection

void material_collection::add(pt::path const& path)
{
    try
    {
        config_node config(path);

        for (auto const& child_config : config)
        {
            if (child_config.key() == "material")
            {
                material_builder builder
                    = create_material_builder(child_config, m_shaders);

                m_builders.insert(m_builders.end(), builder);
            }
            else if (child_config.key() == "light_shader_instances")
            {
                for (auto const& light_shader_instance_config : child_config)
                {
                    assert(light_shader_instance_config.key() == "light_shader_instance");

                    light_material_builder builder = create_light_material_builder(
                        light_shader_instance_config, m_light_shaders);

                    m_light_builders.push_back(builder);
                }
            }
        }
    }
    catch (pt::exceptions::contextual_exception& e)
    {
        //auto context = path_file_context
        e.attach_context(new pt::exceptions::path_file_context(path));
        throw;
    }
}

std::shared_ptr<material const> material_collection::get(
    std::string const& name) const
{
    for (auto const& material : m_materials)
    {
        if (material->name() == name)
            return material;
    }
    throw any_error(format("Material \"%1%\" not found.") % name);
}

std::shared_ptr<material> material_collection::populate(
    parameter_collection const& p3d_parameters,
    parameter_collection const& shared_parameters, graphics_device& device)
{
    std::shared_ptr<material> result;

    if (p3d_parameters.empty())
    {
        material_builder* builder = get_builder_by_name("default");
        builder->build(parameter_collection(), shared_parameters, device);
        result = builder->built_material();
    }

    for (auto const& node : p3d_parameters)
    {
        if (node->name() == "p3d material")
        {
            if (result)
            {
                log("multimaterial..");
                continue;
            }
            std::string p3d_mat_name = constant_value<std::string>(*node);

            material_builder* builder = get_builder_by_p3d_name(p3d_mat_name);

            if (!builder)
            {
                builder = get_builder_by_name("default");
                assert(builder);
            }

            //hackkkk
            if (!builder->built_material())
            {
                builder->build(node->children(), shared_parameters, device);

                result = builder->built_material();
                assert(result);

                m_materials.push_back(result);
            }
            else
            {
                result = builder->built_material();
            }
        }
    }
    assert(result);
    return result;
}

std::shared_ptr<material> material_collection::populate(std::string const& name,
    parameter_collection const& parameters,
    parameter_collection const& shared_parameters, graphics_device& device)
{
    std::shared_ptr<material> result;

    material_builder* builder = get_builder_by_name(name);
    assert(builder);

    builder->build(parameters, shared_parameters, device);

    result = builder->built_material();
    assert(result);

    m_materials.push_back(result);

    return result;
}

std::shared_ptr<light_material> material_collection::populate_light(
    parameter_collection const& p3d_parameters,
    parameter_collection const& shared_parameters,
    graphics_device& device)
{
    std::shared_ptr<light_material> result;

    assert(!p3d_parameters.empty());
    //if (p3d_parameters.empty())
    //{
    //    light_material_builder* builder = get_light_builder_by_name("default");
    //    builder->build(parameter_collection(), shared_parameters, device);
    //    result = builder->built_material();
    //}

    for (auto const& node : p3d_parameters)
    {
        if (node->name() == "light_shader_instance_name")
        {
            if (result)
            {
                log("multimaterial..");
                continue;
            }
            std::string light_material_name = constant_value<std::string>(*node);

            light_material_builder* builder = get_light_builder_by_name(light_material_name);

            if (!builder)
            {
                builder = get_light_builder_by_name("default_point_light");
                assert(builder);
            }

            //hackkkk
            if (!builder->built_material())
            {
                builder->build(node->children(), shared_parameters, device);

                result = builder->built_material();
                assert(result);

                m_light_materials.push_back(result);
            }
            else
            {
                result = builder->built_material();
            }
        }
    }
    assert(result);
    return result;
}


/// Returns material builder which has p3d material linked as source.
material_builder* material_collection::get_builder_by_p3d_name(
    std::string const& name)
{
    for (auto& builder : m_builders)
    {
        if (builder.p3d_material_name() == name)
            return &builder;
    }
    log(format("Warning: Material linked to p3d material \"%1%\" not found.")
        % name);

    return 0;
}

/// Returns material builder which builds materials with \a name.
material_builder* material_collection::get_builder_by_name(
    std::string const& name)
{
    for (auto& builder : m_builders)
    {
        log(format("Material builder: \"%1%\"") % builder.name());
        if (builder.name() == name)
            return &builder;
    }
    log(format("Warning: Material builder for material \"%1%\" not found.")
        % name);

    return 0;
}

/// Returns light material builder which builds materials with \a name.
light_material_builder* material_collection::get_light_builder_by_name(
    std::string const& name)
{
    for (auto& builder : m_light_builders)
    {
        log(format("Light material builder: \"%1%\"") % builder.name());
        if (builder.name() == name)
            return &builder;
    }
    log(format("Warning: Light material builder for material \"%1%\" not found.")
        % name);

    return 0;
}

std::shared_ptr<material> material_collection::try_get(std::string const& name)
{
    for (auto const& material_ptr : m_materials)
    {
        if (material_ptr->name() == name)
            return material_ptr;
    }
	return nullptr;
}

// free functions

namespace {
    /// Recursively loads material descriptions under \a path into \a materials.
    void load_materials(boost::filesystem::path const& path, material_collection& materials)
    {
        // directory_iterator default construction yields past-the-end
        pt::directory_iterator end_iter;

        log(format("load_materials path: \"%1%\" ") % path.string());

        for (pt::directory_iterator itr(path); itr != end_iter; ++itr)
        {
            log(format("->: \"%1%\" ") % itr->string());
                    
            //const std::string *str = (const std::string *)&(*itr);
            //if (strncmp(str->c_str(), "data/materials", strlen("data/materials")) != 0)
            //{
            //	log("break;");
            //	break;
            //}

            if (boost::filesystem::is_directory(*itr))
            {
                load_materials(*itr, materials);
                continue;
            }
            
            if (itr->extension() == ".xml")
            {
                materials.add(pt::path(itr->string()));
            }
        }
    }
} // anonymous namespace

/// Returns all materials under \a path.
std::unique_ptr<material_collection> load_materials(pt::path const& path)
{
    auto new_materials = std::make_unique<material_collection>();
    load_materials(boost::filesystem::path(path.string()), *new_materials);

    return new_materials;
}

}}}} // namespace pt::d3d9::rendering::source
