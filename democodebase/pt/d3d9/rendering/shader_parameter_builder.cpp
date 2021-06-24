#include "shader_parameter_builder.h"
#include "source/material_description.h"
#include "pt/d3d9/shaders/surface_shader.h"
#include "pt/d3d9/constant_texture_2d_source.h"
#include "pt/d3d9/texture_2d_builder.h"
#include "pt/d3d9/texture_2d_value.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/animatable_value_utility.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/log.h"
#include "pt/parameter_node.h"
#include "pt/vector_utility.h"
#include "pt/format.h"

#include "pt/d3d9/texture_2d_data_set_builder.h"
#include "pt/d3d9/texture_2d_source.h"
#include "pt/math/step_curve.h"

namespace pt { namespace d3d9 { namespace rendering {

// shader_parameter_builder

shader_parameter_builder::shader_parameter_builder(
    config_node shader_parameters,
    config_node sampler_states,
    parameter_collection const& shared_parameters,
    graphics_device& device)
: m_shader_parameters(shader_parameters)
, m_sampler_states(sampler_states)
, m_shared_parameters(shared_parameters)
, m_device(&device)
{
}

shader_parameters shader_parameter_builder::build(
    source::shader_description const& shader_description,
    parameter_collection const& parameter_overrides) const
{
    parameter_collection parameters;

    config_node const& shader_parameter_configs = m_shader_parameters;

    for (config_node::const_iterator i = shader_parameter_configs.begin();
        i != shader_parameter_configs.end(); ++i)
    {
        parameters.add(build_parameter(*i, shader_description,
            parameter_overrides));
    }

    shader_parameters shader_parameters(parameters, m_sampler_states);
    return shader_parameters;
}

std::shared_ptr<parameter_node> shader_parameter_builder::build_parameter(
    config_node const& shader_parameter,
    source::shader_description const& shader_description,
    parameter_collection const& parameter_overrides) const
{
    assert(shader_parameter.key() == "parameter");

    std::string const& parameter_name
        = shader_parameter.child("name")->value();

    parameter_collection::const_iterator override_iter
        = parameter_overrides.find_child(parameter_name);
    if (override_iter != parameter_overrides.end())
    {
        log(format("Overriding parameter \"%1%\"") % parameter_name);
        return *override_iter;
    }

    config_node::const_iterator material_parameter
        = shader_description.parameters().find_child(
        parameter_name);

    std::shared_ptr<animatable_value_base> val;

    if (material_parameter == shader_description.parameters().end())
    {
        val = default_value_for(shader_parameter);
    }
    else
    {
        val = value_for(*material_parameter, shader_parameter);
    }

    std::string const& hlsl_name
        = shader_parameter.child("hlsl")->value();
    return std::shared_ptr<parameter_node>(new parameter_node(hlsl_name, val));
}

namespace {
    std::shared_ptr<animatable_value_base> create_constant_texture_value(
        path const& path, graphics_device& device)
    {
        //if (!filesystem::exists(path))
        //    throw detail::material_builder_error(path, "file not found");

		auto texture = default_texture_2d_collection().find(path.string());
        if (!texture)
        {
            texture = load_immutable_texture_2d(device, path);
            default_texture_2d_collection().add(path.string(), texture);
        }

        auto source = std::make_shared<constant_texture_2d_source>(texture);
        return std::make_shared<texture_2d_value>(std::move(source));
    }

    template<typename T>
    std::shared_ptr<animatable_value_base> create_constant_vector(
        int element_count, std::string const& value_text)
    {
        assert(element_count >= 1 && element_count <= 4);
        switch(element_count)
        {
        case 1:
            {
                pt::vector<T,1> value;
                pt::parse_from_text(value, value_text);
                return create_constant_value(value);
            }

        case 2:
            {
                pt::vector<T,2> value;
                pt::parse_from_text(value, value_text);
                return create_constant_value(value);
            }

        case 3:
            {
                pt::vector<T,3> value;
                pt::parse_from_text(value, value_text);
                return create_constant_value(value);
            }

        case 4:
            {
                pt::vector<T,4> value;
                pt::parse_from_text(value, value_text);
                return create_constant_value(value);
            }
        }
        throw any_error("invalid element count");
    }

    std::shared_ptr<animatable_value_base> create_constant_value_from_hlsl(
        std::string const& hlsl_type, std::string const& value_text,
        graphics_device& device)
    {
        if (hlsl_type == "texture2D")
        {
            return create_constant_texture_value(path(value_text), device);
        }

        std::string const numbers("0123456789");
        std::string::size_type const number_pos
            = hlsl_type.find_first_of(numbers);

        if (number_pos != std::string::npos)
        {
            // Vector or matrix type. Matrix type not implemented yet.
            std::string const element_type = hlsl_type.substr(0, number_pos);
            int const element_count = lexical_cast<int>(
                hlsl_type.substr(number_pos));
            if (element_type == "float")
                return create_constant_vector<float>(element_count, value_text);
            if (element_type == "int")
                return create_constant_vector<int>(element_count, value_text);
        }
        else
        {
            if (hlsl_type == "float")
            {
                float value;
                parse_from_text(value, value_text);
                return pt::create_constant_value(value);
            }
            if (hlsl_type == "int")
            {
                int value;
                parse_from_text(value, value_text);
                return pt::create_constant_value(value);
            }
        }
        throw any_error(format(
            "material parameter creation not fully implemented for hlsl "
            "type %1% with value %2%") % hlsl_type % value_text);
    }
} // anonymous namespace

std::shared_ptr<animatable_value_base>
    shader_parameter_builder::value_for(
    config_node const& material_parameter,
    config_node const& shader_parameter) const
{
    std::string const& hlsl_type = shader_parameter.child("type")->value();

    config_node::const_iterator value_iter = material_parameter.find_child("shared_value");
    bool is_shared_value = value_iter != material_parameter.end();
    if (is_shared_value)
    {
        // Link material parameter value to a shared value. No type checking
        // is done.
        parameter_collection::const_iterator shared_iter
            = m_shared_parameters.find_child(value_iter->value());

        if (shared_iter != m_shared_parameters.end())
        {
            std::shared_ptr<animatable_value_base> value_base = (*shared_iter)->value();

            std::shared_ptr<animatable_value<std::string>> string_value
                = std::dynamic_pointer_cast<animatable_value<std::string>>(value_base);

            if (string_value)
            {
                std::shared_ptr<pt::math::step_curve_string_value_source> texture_name_step_curve
                    = std::dynamic_pointer_cast<pt::math::step_curve_string_value_source>(string_value->source());

                d3d9::texture_2d_data_set texture_2d_data_set = d3d9::build_texture_2d_data_set(
                    *texture_name_step_curve,
                    *m_device);

                // Create texture_2d_step_curve
                texture_2d_step_curve step_curve(texture_2d_data_set);

                std::shared_ptr<texture_2d_source> source(new texture_2d_source(step_curve));

                std::shared_ptr<texture_2d_value> animatable_texture_2d(
                    new texture_2d_value(source));

                return animatable_texture_2d;
            }

            return value_base;
        }
        //if (shared_iter == m_shared_parameters.end()
        //    && hlsl_type != "texture2D")
        {
            // This will throw error.
            m_shared_parameters.find_child(value_iter->value());
        }
    }

    std::string const& material_parameter_value_text
        = material_parameter.child("value")->value();

    return create_constant_value_from_hlsl(hlsl_type,
        material_parameter_value_text, *m_device);
}

std::shared_ptr<animatable_value_base>
    shader_parameter_builder::default_value_for(
    config_node const& shader_parameter) const
{
    std::string const& name = shader_parameter.child("name")->value();
    std::string const& hlsl_type = shader_parameter.child("type")->value();
    std::string const& shader_parameter_default_value_text
        = shader_parameter.child("default_value")->value();

    log(std::string("No material parameter for shader parameter \"" + name
        + "\", using default: ") + shader_parameter_default_value_text);

    return create_constant_value_from_hlsl(hlsl_type,
        shader_parameter_default_value_text, *m_device);
}

}}} // namespace pt::d3d9::rendering
