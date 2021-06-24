#include "plasma.h"
#include "rendertarget.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/d3d9/rendering/material_cache.h"
#include "pt/d3d9/rendering/surface.h"
#include "pt/d3d9/rendering/shading_program.h"
#include "pt/d3d9/rendering/shading_program_builder.h"
#include "pt/d3d9/rendering/simple_variable_owner.h"
#include "pt/d3d9/rendering/reference_setter.h"
#include "pt/d3d9/constant_texture_2d_source.h"
#include "pt/d3d9/texture_2d_value.h"
#include "pt/config_node.h"
#include "pt/parameter_collection.h"
#include "pt/parameter_node.h"
#include "pt/any_error.h"

namespace content { namespace sequences {

namespace detail {
    class time_variable : public d3d9::rendering::simple_variable_owner
    {
    public:
        void set_time(float time)
        {   
            //srand(static_cast<unsigned int>(time));
            float rand_x,rand_y;
            int lowest=0, highest=40000;//static_cast<unsigned int>(time);
            int range=(highest-lowest)+1;
    
            rand_x = static_cast<float>((lowest+int(range*rand()/(RAND_MAX + 1.0))) / 40000.0);        
            rand_y = static_cast<float>((lowest+int(range*rand()/(RAND_MAX + 1.0))) / 40000.0);

            m_time_x = time;
            
            m_time_y = time * 0.89f + 0.53f;
			m_time = time;
            if (time) {};
        }
        // Implicit cast operator.
        operator float()
        {
            return m_time_x;
        }
    protected: // interface required by d3d9::rendering::simple_variable_owner
        std::unique_ptr<d3d9::rendering::constant_setter> on_create_constant(
            d3d9::constant const& constant) const
        {
            if (constant.name() == "time_x")
            {
                return std::unique_ptr<d3d9::rendering::constant_setter>(
                    new d3d9::rendering::reference_setter<float>(m_time_x));
            }
            if (constant.name() == "time_y")
            {
                return std::unique_ptr<d3d9::rendering::constant_setter>(
                    new d3d9::rendering::reference_setter<float>(m_time_y));
            }
            if (constant.name() == "time")
            {
                return std::unique_ptr<d3d9::rendering::constant_setter>(
                    new d3d9::rendering::reference_setter<float>(m_time));
            }

            return nullptr;
        }

    private: // data members
        float m_time_x;
        float m_time_y;
		float m_time;
    };
} // namespace detail

plasma::plasma(d3d9::graphics_device& device, config_node const& config,
    d3d9::rendering::material_cache& materials, pt::parameter_collection const& parameters)
: simple_composite_sequence(config)
, m_device(&device)
{
    assert(config.key() == "plasma");

    std::string material_name;
    config.parse_child("material_name", material_name);

    parameter_collection new_parameters = parameters;

    m_child_texture_source.reset(new d3d9::constant_texture_2d_source);
    m_child_texture.reset(new d3d9::texture_2d_value(m_child_texture_source));
    new_parameters.add(std::shared_ptr<parameter_node>(new parameter_node("child_texture", m_child_texture)));

    m_surface = materials.find_or_create(device, material_name, new_parameters);

    m_program = d3d9::rendering::create_shared_shading_program(
        device, *m_surface->instances()[0].shader(), m_dummy_light_shader);

    m_time.reset(new detail::time_variable);
}


std::shared_ptr<d3d9::texture_2d> plasma::on_draw(rendertarget& target,
    float time, std::shared_ptr<d3d9::texture_2d> child_result)
{
    if (target.texture() == child_result)
    {
        throw any_error("Plasma: texture from input sequence is identical to"
            " given target.");
    }

    scoped_optional_rendertarget scoped_target(target);

    m_child_texture_source->set_texture(child_result);
    //m_child_texture->texture_source()->set_texture(child_result);

    {
        m_time->set_time(time);

        std::vector<d3d9::rendering::variable_owner*> owners;
        owners.push_back(&m_surface->instances()[0].variables());
        owners.push_back(m_time.get());

        m_program->create_resources(owners);
    }

    {
        std::vector<d3d9::rendering::variable_owner const*> owners;
        owners.push_back(&m_surface->instances()[0].variables());
        owners.push_back(m_time.get());

        m_program->prepare(owners, *m_time);
    }
	
    m_program->use();

    //vector2f min = center - (size * 0.5f);
    //vector2f max = min + size;
    //d3d9::rendering::fill_quad(device, min, max);

    d3d9::rendering::fill_fullscreen_quad(*m_device);

    return nullptr;
}

}} // namespace content::sequences
