#include "scene_player.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/rendering/camera.h"
#include "pt/d3d9/rendering/draw_calls.h"
#include "pt/d3d9/rendering/drawable.h"
#include "pt/d3d9/rendering/geometry.h"
#include "pt/d3d9/rendering/item_collection.h"
#include "pt/d3d9/rendering/item_geometry.h"
#include "pt/d3d9/rendering/item_name.h"
#include "pt/d3d9/rendering/scene.h"
#include "pt/d3d9/rendering/renderer.h"
#include "pt/d3d9/rendering/view.h"

#include "pt/d3d9/rendering/light.h"
#include "pt/d3d9/rendering/light_properties.h"
#include "pt/d3d9/rendering/program_cache.h"
#include "pt/d3d9/rendering/projection_matrix.h"
#include "pt/d3d9/rendering/transformer.h"
#include "pt/d3d9/rendering/transformation_variables.h"
#include "pt/d3d9/rendering/post_processing.h"

#include "pt/d3d9/graphics_device.h"

#include "pt/log.h"
#include "pt/string_wrapper.h"

#include "../application.h"

#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/texture_2d_utils.h"

#include "pt/d3d9/scoped_convenience.h"
#include "pt/parameter_collection.h"

namespace content {

namespace {
	std::shared_ptr<d3d9::rendering::program_cache> program_cache;
} // anonymous namespace

std::shared_ptr<d3d9::rendering::program_cache> global_program_cache(
    d3d9::graphics_device& device)
{
    if (!program_cache)
        program_cache.reset(new d3d9::rendering::program_cache(device));

    return program_cache;
}

void clear_global_program_cache()
{
    program_cache.reset();
}

scene_player::scene_player(d3d9::graphics_device& device,
    std::shared_ptr<d3d9::rendering::scene> scene)
: d3d9::device_reference(device)
, m_scene(scene)
, m_item_with_view(0)
, m_last_update_time(-1.0f)
{
    assert(m_scene);

    if (!program_cache)
        program_cache.reset(new d3d9::rendering::program_cache(device));
}

scene_player::~scene_player()
{
    pt::log("Destructing scene_player." + m_scene->name().text());
}

void scene_player::draw_with_preshader() const
{
    if (!m_item_with_view)
        return;

    d3d9::graphics_device& device = mutable_device();

    std::vector<d3d9::rendering::draw_call> opaque_calls
        = d3d9::rendering::gather_opaque_draw_calls(*m_scene, m_last_update_time);
    d3d9::rendering::sort_draw_calls(opaque_calls);

    //Draw scene without lights to render targets to get position and normals
    {
        d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, true);
        //d3d9::set_render_state(device, D3DRS_COLORWRITEENABLE, 0x00000000);

        d3d9::rendering::render(opaque_calls, m_last_update_time, nullptr,
            &m_item_with_view->camera(), *program_cache, device, true);

    }
    
    d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, false);
    //d3d9::set_render_state(device, D3DRS_COLORWRITEENABLE, 0x0000000F);
}

void scene_player::set_shadow_depth_program(std::shared_ptr<d3d9::rendering::shading_program> program)
{	
	m_shadow_depth_program = program;
}

void scene_player::draw_to_shadowmap(
    //d3d9::rendering::light *light,
    d3d9::rendering::item* item_with_light,
    std::vector<d3d9::rendering::draw_call>& calls)
{
	//set_texture(mutable_device(), 0, com_ptr<IDirect3DTexture9>());
	{
		std::shared_ptr<d3d9::texture_2d> shadow_depth_texture = d3d9::default_texture_2d_collection().find("ShadowTexture");
		d3d9::scoped_render_target_texture scoped_target_texture(*shadow_depth_texture);
		d3d9::graphics_device& device = shadow_depth_texture->mutable_device();
		com_ptr<IDirect3DSurface9> shadow_zbuffer = d3d9::get_depth_stencil(mutable_device(), shadow_depth_texture->resolution());
		d3d9::scoped_depth_stencil scopeddsgtf(device, shadow_zbuffer);
		//d3d9::scoped_optional_rendertarget_texture scoped_target_texture(shadow_depth_texture);


		/*
				d3d9::scoped_texture_stage_state color_op(device, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
				d3d9::scoped_texture_stage_state alpha_op(device, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

				d3d9::set_render_state(device, D3DRS_LIGHTING, FALSE);
				d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, FALSE);
				d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_ONE);
				d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_ZERO);

				d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, true);
				d3d9::set_render_state(device, D3DRS_COLORWRITEENABLE, 0x0000000F);
			*/	
		//d3d9::scoped_render_state(device, D3DRS_ZWRITEENABLE, true);
		//d3d9::scoped_render_state(device, D3DRS_ALPHABLENDENABLE, FALSE);
		//d3d9::scoped_render_state(device, D3DRS_SRCBLEND, D3DBLEND_ONE);
		//d3d9::scoped_render_state(device, D3DRS_DESTBLEND, D3DBLEND_ZERO);
        //d3d9::scoped_render_state(device, D3DRS_DESTBLEND, D3DBLEND_ZERO);
		d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, true);
		DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		//vector4f clear_color(1, 1, 1, 1);
        vector4f clear_color(0, 0, 0, 0);
        d3d9::clear(device, flags, clear_color, 1.0f, 0);


        auto& camera_item = d3d9::rendering::get_item_with_camera(item_with_light->name(), *m_scene);
        d3d9::rendering::view& light_view = camera_item.camera();
		//d3d9::rendering::camera& light_view = d3d9::rendering::get_camera(light->name(), *m_scene);
        
        item_with_light->light().projection()->set_resolution(shadow_depth_texture->resolution());
		light_view.projection().set_resolution(shadow_depth_texture->resolution());
		//TODO: Change near and far planes to be fetched somewhere
		//light_view.projection().set_zplanes(1.0f, 200.0f);
		//light_view.projection().update(m_last_update_time);
        d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, D3DCULL_CW);
        
        d3d9::rendering::render_to_shadowmap(calls, m_last_update_time,
			&light_view, m_shadow_depth_program);
	}
	//set_texture(mutable_device(), 0, com_ptr<IDirect3DTexture9>());
	d3d9::set_render_state(mutable_device(), D3DRS_ZWRITEENABLE, false);

}
//void scene_player::draw()
//{
//	content::sequences::rendertarget *target = NULL;
//	draw(*target);
//}

void scene_player::draw(content::sequences::rendertarget& target)
{
    if (!m_item_with_view)
        return;
	
    d3d9::graphics_device& device = mutable_device();
	target.activate();

	d3d9::set_texture_stage_state(device, 0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	d3d9::set_texture_stage_state(device, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	d3d9::set_render_state(device, D3DRS_LIGHTING, FALSE);

    std::vector<d3d9::rendering::draw_call> opaque_calls
        = d3d9::rendering::gather_opaque_draw_calls(*m_scene, m_last_update_time);
    d3d9::rendering::sort_draw_calls(opaque_calls);

    // Z pre-pass
    {
        d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, true);
        d3d9::set_render_state(device, D3DRS_COLORWRITEENABLE, 0x00000000);

        d3d9::rendering::render_depth(opaque_calls, m_last_update_time,
            &m_item_with_view->camera(), *program_cache, device);
        
    }
    
    d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, false);
    d3d9::set_render_state(device, D3DRS_COLORWRITEENABLE, 0x0000000F);

    // Draw oaque items with lights.
    int drawn_lights = 0;
    size_t const light_count = m_scene->light_count();
    for (size_t i = 0 ; i < light_count; ++i)
    {
        auto item_with_light = m_scene->lights()[i];
        // TODO: this should be const?
        d3d9::rendering::light& light = item_with_light->light();
		
		//Draw to shadowmap from lights perspective
        if (light.is_shadow_mapped())
		{
            d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, FALSE);
            d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
            d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

            draw_to_shadowmap(item_with_light, opaque_calls);

            // Re-enable passed target.
			target.activate();
		}
        else
        {
            //continue;
        }

		//Draw all objects
        bool is_additive_enabled = drawn_lights != 0;
        if (!is_additive_enabled)
        {
            d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, FALSE);
            d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
            d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
        }
        else
        {
            // Rest of lights as additive.
            d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, TRUE);
            d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_ONE);
            d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_ONE);
        }

        d3d9::rendering::render(opaque_calls, m_last_update_time, &light,
            &m_item_with_view->camera(), *program_cache, device);
        ++drawn_lights;
    }

    d3d9::set_render_state(mutable_device(), D3DRS_ALPHABLENDENABLE, FALSE);
    d3d9::set_render_state(mutable_device(), D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
    d3d9::set_render_state(mutable_device(), D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

    // Draw additive and blended materials in one pass, sorted by depth.

    std::vector<d3d9::rendering::draw_call> additive_calls
        = d3d9::rendering::gather_additive_draw_calls(*m_scene, m_last_update_time);

    std::vector<d3d9::rendering::draw_call> translucent_calls
        = d3d9::rendering::gather_transparent_draw_calls(*m_scene, m_last_update_time);

    translucent_calls.insert(translucent_calls.end(), additive_calls.begin(), additive_calls.end());

    if (!translucent_calls.empty())
    {
        d3d9::rendering::sort_transparent_draw_calls(*m_item_with_view,
            m_last_update_time, translucent_calls);

        d3d9::rendering::render_transparents(
            translucent_calls,
            m_last_update_time,
            &m_item_with_view->camera(),
            *program_cache,
            m_scene->lights(),
            m_scene->light_count(),
            device);
    }
}

void scene_player::update(float time)
{
    // hack.
    if (!m_item_with_view && m_scene->camera_count() > 0)
        set_view_at(0);

    assert(m_item_with_view);

    if (m_last_update_time == time)
        return;

    m_last_update_time = time;
    //d3d9::rendering::update_recursively(*m_scene, time, matrix4x4f());

    size_t const light_count = m_scene->light_count();

    // Need to update scene without lights anyways, to prepare shading programs with dummy light shader.
    for (std::shared_ptr<d3d9::rendering::item> item : m_scene->children())
        update_item(*item, 0);

    // Update
    for (size_t i = 0; i < light_count; ++i)
    {
        auto item_with_light = m_scene->lights()[i];

        for (std::shared_ptr<d3d9::rendering::item> item : m_scene->children())
            update_item(*item, item_with_light);
    }
}

void scene_player::set_view(d3d9::rendering::item* item_with_view)
{
    m_item_with_view = item_with_view;
}

void scene_player::set_view(d3d9::rendering::item* item_with_view, float near_plane, float far_plane)
{
    m_item_with_view = item_with_view;

    m_item_with_view->camera().projection().set_zplanes(near_plane, far_plane);
}

void scene_player::set_view(std::string const& name)
{
    m_item_with_view = &d3d9::rendering::get_item_with_camera(name, *m_scene);

    m_item_with_view->camera().projection().set_resolution(content::application::resolution());
}

void scene_player::set_view(std::string const& name, float near_plane, float far_plane)
{
    m_item_with_view = &d3d9::rendering::get_item_with_camera(name, *m_scene);

    m_item_with_view->camera().projection().set_resolution(content::application::resolution());
    m_item_with_view->camera().projection().set_zplanes(near_plane, far_plane);
}

void scene_player::set_view_at(size_t index)
{
    assert(index < m_scene->camera_count());
    m_item_with_view = m_scene->cameras()[index];
}

void scene_player::update_item(d3d9::rendering::item& item,
    d3d9::rendering::item* item_with_light)
{
    if (item.has_geometry())
    {
        auto& surface = item.geometry().geometry().surface();

        std::vector<d3d9::rendering::variable_owner*> owners;
        owners.reserve(8);

        owners.push_back(&item.geometry().variables());
		//owners.push_back(&item.transformer().variables());

        if (item_with_light && item_with_light->light().is_shadow_mapped())
		{
            auto& item_with_camera = d3d9::rendering::get_item_with_camera(item_with_light->name(), *m_scene);
            d3d9::rendering::view& light_view = item_with_camera.camera();
			owners.push_back(&light_view.variables());
            d3d9::rendering::update_shadow_depth(surface, m_shadow_depth_program, owners);
		} 

        if (item_with_light)
        {
            owners.push_back(&item_with_light->light().variables());
            owners.push_back(&item_with_light->light().light_properties().variables());
        }
		owners.push_back(&m_item_with_view->camera().variables());

        d3d9::rendering::update(surface, item_with_light, *program_cache, owners);
    }

    for (std::shared_ptr<d3d9::rendering::item> child_item : item.children())
    {
        update_item(*child_item, item_with_light);
    }
}

d3d9::rendering::item* scene_player::get_item_with_view() {
    return this->m_item_with_view;
}

} // namespace content
