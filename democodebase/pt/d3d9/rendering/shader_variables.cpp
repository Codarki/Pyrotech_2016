#include "shader_variables.h"
#include "parameter_node_setter_create.h"
#include "sampler_setter.h"
#include "constant_setter.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/sampler_state.h"
#include "pt/d3d9/texture_2d_value.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/format.h"
#include "pt/parameter_node_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

// shader_parameters

shader_parameters::shader_parameters(parameter_collection const& parameters,
    config_node const& sampler_states)
: m_parameters(parameters)
, m_sampler_states(sampler_states)
{
}

parameter_collection const& shader_parameters::parameters() const
{
    return m_parameters;
}

config_node const& shader_parameters::sampler_states() const
{
    return m_sampler_states;
}

// shader_variables

shader_variables::shader_variables(
    shader_parameters const& parameters)
: m_parameters(parameters)
{
}

shader_variables::~shader_variables()
{
}

parameter_collection const& shader_variables::parameters() const
{
    return m_parameters.parameters();
}

std::unique_ptr<constant_setter> shader_variables::on_create_constant(
    constant const& constant) const
{
    if (constant.desc().Class == D3DXPC_SCALAR)
        return create_scalar(constant);

    if (constant.desc().Class == D3DXPC_VECTOR)
        return create_vector(constant);

    return nullptr;
}

std::unique_ptr<variable_setter> shader_variables::on_create_setter(
    constant_buffer const& constants, size_t index) const
{
    constant const& c = constants.at(index);

    if (c.desc().Class == D3DXPC_OBJECT)
        return create_object(constants, c);

    return nullptr;
}

std::unique_ptr<variable_setter> shader_variables::create_object(
    constant_buffer const& constants, constant const& c) const
{
    assert(c.desc().Class == D3DXPC_OBJECT);

    if (c.desc().Type == D3DXPT_SAMPLER2D)
    {
        config_node const& sampler_config
            = m_parameters.sampler_states();

        for (config_node::const_iterator i = sampler_config.begin();
            i != sampler_config.end(); ++i)
        {
            if (i->key() != "sampler")
                continue;

            if (i->child("name")->value() == c.name())
            {
                D3DXHANDLE h = c.handle();
                UINT index = constants.native()->GetSamplerIndex(h);

                std::string texture_name = i->child("texture")->value();

                parameter_collection::const_iterator texture_iter
                    = m_parameters.parameters().find_child(texture_name);
                parameter_collection::const_iterator end_iter
                    = m_parameters.parameters().end();
                assert(texture_iter != end_iter);
                if (texture_iter == end_iter)
                {
                    throw any_error(
                        format("No texture was created for parameter '%1%'") % texture_name);
                }

                sampler_state sampler(constants.mutable_device(), index);
                sampler.parse(*i);

                parameter_node& texture_node = **texture_iter;
                std::shared_ptr<animatable_value_base> texture_value_base = texture_node.value();

                //

                //animatable_value<      boost::shared_ptr<texture_2d>     >
                //animatable_value<      boost::shared_ptr<texture_2d>     >

                //

                std::shared_ptr<texture_2d_value> texture = std::dynamic_pointer_cast<texture_2d_value>(texture_value_base);
                assert(texture);

                return std::unique_ptr<variable_setter>(
                    new sampler_setter(sampler, texture));
            }
        }
    }

    return nullptr;
}

std::unique_ptr<constant_setter> shader_variables::create_scalar(
    constant const& constant) const
{
    assert(constant.desc().Class == D3DXPC_SCALAR);

    parameter_collection::const_iterator child
        = m_parameters.parameters().find_child(constant.name());

    if (child != m_parameters.parameters().end())
        return create_parameter_node_setter(*child, constant);

    return nullptr;
}

std::unique_ptr<constant_setter> shader_variables::create_vector(
    constant const& constant) const
{
    assert(constant.desc().Class == D3DXPC_VECTOR);

    parameter_collection::const_iterator child
        = m_parameters.parameters().find_child(constant.name());

    if (child != m_parameters.parameters().end())
        return create_parameter_node_setter(*child, constant);

    return nullptr;
}

}}} // namespace pt::d3d9::rendering
