#include "animated_texture.h"
#include "basic_composite_sequence.h"
#include "rendertarget.h"
#include "draw_to_texture.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/d3d9/rendering/material_cache.h"
#include "pt/d3d9/rendering/surface.h"
#include "pt/d3d9/rendering/shading_program.h"
#include "pt/d3d9/rendering/shading_program_builder.h"
#include "pt/d3d9/rendering/simple_variable_owner.h"
#include "pt/d3d9/rendering/reference_setter.h"
#include "pt/d3d9/rendering/renderer.h" // for dummy light shader
#include "pt/d3d9/constant_texture_2d_source.h"
#include "pt/d3d9/texture_2d_value.h"
#include "pt/config_node.h"
#include "pt/parameter_collection.h"
#include "pt/parameter_node.h"
#include "pt/any_error.h"

namespace content { namespace sequences {

using namespace pt;

class animated_texture::impl
{
public:
    impl(
        pt::config_node const& config,
        pt::parameter_collection const& parameters,
        pt::d3d9::graphics_device& device,
        d3d9::rendering::material_cache& materials)
    : m_basic(config,parameters)
    , m_device(&device)
    {
        assert(config.key() == "animated_texture");

        m_target_texture = find_shared_texture(config, parameters, "target_shared_texture");

        std::string material_name;
        config.parse_child("material_name", material_name);

        //parameter_collection tmp_parameters;
        //m_child_texture_source.reset(new d3d9::constant_texture_2d_source);
        //m_child_texture.reset(new d3d9::texture_2d_value(m_child_texture_source));
        //tmp_parameters.add(std::shared_ptr<parameter_node>(new parameter_node("child_texture", m_child_texture)));

        m_surface = materials.find_or_create(device, material_name, parameters);

        m_program = d3d9::rendering::create_shared_shading_program(
            device,
            *m_surface->instances()[0].shader(),
            d3d9::rendering::global_dummy_light_shader());

        //m_time.reset(new detail::time_variable);
    }

    pt::string_wrapper const& name() const
    {
        return m_basic.name();
    }

    std::shared_ptr<pt::d3d9::texture_2d> draw(rendertarget&, float time)
    {
        //scoped_optional_rendertarget scoped_target(target);
        d3d9::scoped_render_target_texture scoped_target(*m_target_texture);

        //m_child_texture_source->set_texture(child_result);
        //m_child_texture->texture_source()->set_texture(child_result);

        // Create d3d9 resources if not created yet.
        {
            //m_time->set_time(time);

            std::vector<d3d9::rendering::variable_owner*> owners;
            owners.push_back(&m_surface->instances()[0].variables());
            //owners.push_back(m_time.get());

            m_program->create_resources(owners);
        }

        // Update all d3d9 resources.
        {
            std::vector<d3d9::rendering::variable_owner const*> owners;
            owners.push_back(&m_surface->instances()[0].variables());
            //owners.push_back(m_time.get());

            // hack to get material animations in sync with 30fps scenes.
            time *= 30.0f;

            m_program->prepare(owners, time);
        }
	
        m_program->use();

        d3d9::rendering::fill_fullscreen_quad(*m_device);

		return nullptr;
    }

    float start_time() const
    {
        return m_basic.start_time();
    }

    float end_time() const
    {
        return m_basic.end_time();
    }

    void set_override_view(pt::d3d9::rendering::item* view)
    {
        m_basic.set_override_view(view);
    }

    void link_inputs(composite_sequence_collection const& sequences)
    {
        m_basic.link_inputs(sequences);
    }

private:
    basic_composite_sequence m_basic;
    std::shared_ptr<d3d9::texture_2d> m_target_texture;
    d3d9::graphics_device* m_device;

    std::shared_ptr<d3d9::rendering::shading_program> m_program;
    std::shared_ptr<d3d9::rendering::surface> m_surface;

    //std::shared_ptr<d3d9::constant_texture_2d_source> m_child_texture_source;
    //std::shared_ptr<d3d9::texture_2d_value> m_child_texture;
};

animated_texture::animated_texture(
    pt::config_node const& config,
    pt::parameter_collection const& parameters,
    pt::d3d9::graphics_device& device,
    d3d9::rendering::material_cache& materials)
: m_impl(new impl(config, parameters, device, materials))
{
}

pt::string_wrapper const& animated_texture::name() const
{
    return m_impl->name();
}

std::shared_ptr<pt::d3d9::texture_2d> animated_texture::draw(rendertarget& target, float time)
{
    return m_impl->draw(target, time);
}

float animated_texture::start_time() const
{
    return m_impl->start_time();
}

float animated_texture::end_time() const
{
    return m_impl->end_time();
}

void animated_texture::set_override_view(pt::d3d9::rendering::item* view)
{
    m_impl->set_override_view(view);
}

void animated_texture::link_inputs(composite_sequence_collection const& sequences)
{
    m_impl->link_inputs(sequences);
}

}} // namespace content::sequences
