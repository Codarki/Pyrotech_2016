#include "scene_sequence.h"
#include "scene_cache.h"
#include "../scene_player.h"
#include "../utility.h"
#include "pt/config_node.h"
#include "pt/parameter_collection.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/texture_2d_utils.h"
#include "pt/d3d9/texture_2d_value_utils.h"
#include "pt/d3d9/rendering/scene_builder.h"
#include "pt/d3d9/graphics_device.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/rendering/material_cache.h"
#include "pt/d3d9/rendering/post_processing.h"
#include "pt/d3d9/rendering/camera.h"
#include "pt/d3d9/rendering/blur.h"
#include "pt/d3d9/rendering/composite_curve_description_collection.h"
#include "pt/d3d9/rendering/renderer.h" // for dummy light shader
#include "pt/path.h"

namespace content { namespace sequences {

scene_sequence::scene_sequence(d3d9::graphics_device& device,
    config_node const& config, parameter_collection parameters,
    d3d9::rendering::material_cache& materials,
    sequences::scene_cache& scene_cache,
    progress_report* progress,
    bool only_serialize)
: m_basic(config, parameters)
, m_materials(materials)
, m_override_view(0)
, m_is_half_resolution(false)
{
    assert(config.key() == "scene_sequence");

    std::string scene_name;
    config.parse_child("scene", scene_name);
    //config.parse_child("camera", m_camera_name);
    config.parse_optional_child("frames_per_second", m_fps, 30);
    config.parse_child("start_time", m_start_time);
    config.parse_child("start_frame", m_start_frame);
    config.parse_child("end_frame", m_end_frame);

    config.parse_optional_child("near_plane", m_near_clip_plane, 1.0f);
    config.parse_optional_child("far_plane", m_far_clip_plane, 1000.0f);
    config.parse_optional_child("ssao_enabled", m_ssao_enabled, false);

    std::shared_ptr<parameter_node> curves = *parameters.child("curves");
    m_camera_name = create_curve_value<std::string>(
        *config.child("camera"), curves);

    config.parse_optional_child("is_half_resolution", m_is_half_resolution, false);

    pt::d3d9::rendering::composite_curve_description_collection composite_curves;
    auto item_curves = config.find_child("item_curves");
    if (item_curves != config.end())
    {
        for (auto it = item_curves->begin(), it_end = item_curves->end(); it != it_end; ++it)
        {
            assert(it->key() == "composite");
            std::string target = it->child("target")->value();
            std::string curve = it->child("curve")->value();

            composite_curves.add(target, curve);
        }
    }

    m_path = pt::path(scene_name);

    //d3d9::rendering::scene_builder builder(device, m_path, m_materials,
    //    progress);
    //m_scene = builder.built_scene();
    
    m_scene = scene_cache.find_or_load(scene_name, device, materials, progress, only_serialize, composite_curves);

    //// ARR light boxes
    //set_light_boxes(device, m_scene, m_materials);
    m_player.reset(new scene_player(device, m_scene));

    vector2i resolution(1024, 1024);

    parameter_collection shader_params;	
    m_ssao_surface = m_materials.find_or_create(device, "ssao_buffer_material", shader_params);

    //m_ssao_program.reset(new d3d9::rendering::shading_program(device,m_ssao_surface->shader()));

    std::shared_ptr<d3d9::rendering::program_cache> program_cache = global_program_cache(device);
    m_ssao_program = program_cache->find_or_create(*m_ssao_surface->instances()[0].shader(),
        d3d9::rendering::global_dummy_light_shader());

    std::shared_ptr<d3d9::texture_2d> texture
        = find_or_load("data/images/common/white.tga", device);

    std::shared_ptr<d3d9::texture_2d_value> blur_strength = d3d9::create_constant_texture_2d_value(texture);

    m_blur.reset(new d3d9::rendering::blur(device, resolution, D3DFMT_A8R8G8B8, blur_strength));
	
	//Init depth material and depth shader for shadow mapping
	parameter_collection shader_params_2;
	m_shadow_depth_surface = m_materials.find_or_create(device, "depth_material", shader_params_2);
    m_shadow_depth_program = program_cache->find_or_create(
        *m_shadow_depth_surface->instances()[0].shader(),
        d3d9::rendering::global_dummy_light_shader());
}

string_wrapper const& scene_sequence::name() const
{
    return m_basic.name();
}

std::shared_ptr<d3d9::texture_2d> scene_sequence::draw(rendertarget& target,
    float time)
{
    //float time_to_start = m_start_frame / m_fps;
    float frame = time * m_fps;

    std::string camera_name = m_camera_name->value_at(time);

    if (m_override_view)
        m_player->set_view(m_override_view, m_near_clip_plane, m_far_clip_plane);
    else if (!camera_name.empty())
        m_player->set_view(camera_name, m_near_clip_plane, m_far_clip_plane);
    else
        m_player->set_view(nullptr);

	m_player->set_shadow_depth_program(m_shadow_depth_program);

    //m_player->update(frame + time_to_start);
    m_player->update(frame + m_start_frame);

    //Use preshader and render position and normal to texture
    {
        set_texture(m_player->device(), 0, com_ptr<IDirect3DTexture9>());
        set_texture(m_player->device(), 1, com_ptr<IDirect3DTexture9>());

        std::shared_ptr<d3d9::texture_2d> ssao_position_rendertexture;
        std::shared_ptr<d3d9::texture_2d> ssao_normal_rendertexture;

        if (m_is_half_resolution)
        {
            ssao_position_rendertexture = d3d9::default_texture_2d_collection().find("DepthTexture_half");
            ssao_normal_rendertexture = d3d9::default_texture_2d_collection().find("NormalTexture_half");
        }
        else
        {
            ssao_position_rendertexture = d3d9::default_texture_2d_collection().find("DepthTexture");
            ssao_normal_rendertexture = d3d9::default_texture_2d_collection().find("NormalTexture");
        }

        d3d9::scoped_render_target_texture scoped_target_texture(*ssao_position_rendertexture);
        d3d9::scoped_render_target_texture scoped_target_texture2(*ssao_normal_rendertexture,0,1);
        
        DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
        vector4f clear_color(0, 0, 0, 0);
        d3d9::clear(m_player->device(), flags, clear_color, 1.0f, 0);
    
        d3d9::scoped_texture_stage_state color_op(m_player->device(), 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
        d3d9::scoped_texture_stage_state alpha_op(m_player->device(), 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        d3d9::scoped_texture_stage_state color_op1(m_player->device(), 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        d3d9::scoped_texture_stage_state alpha_op1(m_player->device(), 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        
        d3d9::scoped_render_state lightning(m_player->device(), D3DRS_LIGHTING, FALSE);
        
        m_player->draw_with_preshader();

        set_texture(m_player->device(), 0, com_ptr<IDirect3DTexture9>());
        set_texture(m_player->device(), 1, com_ptr<IDirect3DTexture9>());
    }

    
    //Here we need to implement preprocessing step where we are using DepthTexture and NormalTexture
    //for example render to SSAO Buffer
    if (m_ssao_enabled)
    {
        set_texture(m_player->device(), 0, com_ptr<IDirect3DTexture9>());
        set_texture(m_player->device(), 1, com_ptr<IDirect3DTexture9>());

        std::shared_ptr<d3d9::texture_2d> ssao_buffer_rendertexture = d3d9::default_texture_2d_collection().find("SSAOBuffer");
        d3d9::scoped_render_target_texture scoped_target_texture(*ssao_buffer_rendertexture);
        
        d3d9::scoped_texture_stage_state color_op(m_player->device(), 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
        d3d9::scoped_texture_stage_state alpha_op(m_player->device(), 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        d3d9::scoped_texture_stage_state color_op1(m_player->device(), 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        d3d9::scoped_texture_stage_state alpha_op1(m_player->device(), 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        
        d3d9::scoped_render_state lightning(m_player->device(), D3DRS_LIGHTING, FALSE);


        DWORD flags = D3DCLEAR_TARGET;// | D3DCLEAR_ZBUFFER;
        vector4f clear_color(1, 1, 1, 1);
        d3d9::clear(m_player->device(), flags, clear_color, 1.0f, 0);
        
        {
            //m_time->set_time(time);
            
            std::vector<d3d9::rendering::variable_owner*> owners;
            owners.push_back(&m_ssao_surface->instances()[0].variables());
            //owners.push_back(time);
            d3d9::rendering::item* item_with_view = m_player->get_item_with_view();

            owners.push_back(&item_with_view->camera().variables());
            m_ssao_program->create_resources(owners);
        }

        {
            std::vector<d3d9::rendering::variable_owner const*> owners;
            owners.push_back(&m_ssao_surface->instances()[0].variables());
            //owners.push_back(m_time.get());
            d3d9::rendering::item* item_with_view = m_player->get_item_with_view();

            owners.push_back(&item_with_view->camera().variables());

            m_ssao_program->prepare(owners, frame + m_start_frame);
        }

        m_ssao_program->use();
        d3d9::rendering::fill_fullscreen_quad(m_player->device());

        set_texture(m_player->device(), 0, com_ptr<IDirect3DTexture9>());
        set_texture(m_player->device(), 1, com_ptr<IDirect3DTexture9>());
        set_texture(m_player->device(), 2, com_ptr<IDirect3DTexture9>());

    }
	else
	{
        std::shared_ptr<d3d9::texture_2d> ssao_buffer_rendertexture = d3d9::default_texture_2d_collection().find("SSAOBuffer");
        d3d9::scoped_render_target_texture scoped_target_texture(*ssao_buffer_rendertexture);
        DWORD flags = D3DCLEAR_TARGET;// | D3DCLEAR_ZBUFFER;
        vector4f clear_color(1, 1, 1, 0);
        d3d9::clear(ssao_buffer_rendertexture->device(), flags, clear_color, 1.0f, 0);
        
    }

    //Blur the SSAOBuffer
    if (m_ssao_enabled)
    {
        std::shared_ptr<d3d9::texture_2d> ssao_buffer = d3d9::default_texture_2d_collection().find("SSAOBuffer");
        m_blur->draw(*ssao_buffer);

        d3d9::scoped_render_target_texture scoped_target_texture(*ssao_buffer);

        set_texture(m_blur->result(),0);


        d3d9::rendering::fill_fullscreen_quad(m_player->device());

        set_texture(m_player->device(), 0, com_ptr<IDirect3DTexture9>());
    }
    
    //Render normally
    {
        //sequences::scoped_optional_rendertarget scoped_target(target);
        ////DWORD flags = D3DCLEAR_TARGET;
        ////vector4f clear_color(0, 0, 0, 1);
        ////d3d9::clear(m_player->device(), flags, clear_color, 1.0f, 0);

        //d3d9::scoped_texture_stage_state color_op(m_player->device(), 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
        //d3d9::scoped_texture_stage_state alpha_op(m_player->device(), 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

        //d3d9::scoped_render_state lightning(m_player->device(), D3DRS_LIGHTING, FALSE);

        m_player->draw(target);
    }
    

    //   rendertarget ssao_position_render_target(ssao_position_rendertexture->device(), ssao_position_rendertexture);
    //(ssao_position_rendertexture->device()).native()->SetRenderTarget(0,d3d9::surface_level(*ssao_position_rendertexture,0).get());
    //d3d9::graphics_device *device = (d3d9::graphics_device *)&ssao_position_rendertexture->device();
    //ssao_position_rendertexture.get()->device()->SetRenderTarget(1,d3d9::surface_level(*ssao_position_rendertexture,0));

    //sequences::scoped_optional_rendertarget scoped_target_ssao_position(ssao_position_render_target,1);
    //m_player->draw();
    //(ssao_position_rendertexture->device()).native()->SetRenderTarget(1,NULL);
    
    return target.texture();
}

float scene_sequence::start_time() const
{
    return m_start_time;
}

float scene_sequence::end_time() const
{
    float frame_count = m_end_frame - m_start_frame;

    assert(m_fps != 0);
    assert(frame_count > 0);

    float duration = frame_count / m_fps;
    return m_start_time + duration;
}

void scene_sequence::set_override_view(d3d9::rendering::item* view)
{
    m_override_view = view;
}

void scene_sequence::link_inputs(composite_sequence_collection const& sequences)
{
    m_basic.link_inputs(sequences);
}

}} // namespace content::sequences
