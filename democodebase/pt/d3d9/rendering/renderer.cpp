#include "renderer.h"
#include "draw_calls.h"
#include "drawable.h"
#include "geometry.h"
#include "item_geometry.h"
#include "light.h"
#include "light_properties.h"
#include "program_cache.h"
#include "projection_matrix.h"
#include "shading_program.h"
#include "surface.h"
#include "transformation_variables.h"
#include "transformer.h"
#include "variable_owner.h"
#include "view.h"
#include "pt/d3d9/shaders/point_light_shader.h"
#include "pt/d3d9/device_convenience.h"

// hack for cull mode
#include "pt/d3d9/shaders/surface_shader.h"
#include "pt/d3d9/scoped_convenience.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    int flip_cull_mode(int cull_mode)
    {
        if (cull_mode == D3DCULL_NONE)
            return D3DCULL_NONE;

        if (cull_mode == D3DCULL_CW)
            return D3DCULL_CCW;

        if (cull_mode == D3DCULL_CCW)
            return D3DCULL_CW;

        // TODO: throw
        return D3DCULL_CCW;
    }

    int culling_mode(shaders::surface_shader const& shader, view const& view)
    {
        auto cull_mode = shader.d3d_cullmode();
        if (view.flip_culling())
            cull_mode = flip_cull_mode(cull_mode);
        return cull_mode;
    }
}

void render_depth(std::vector<draw_call> const& calls, float time,
    view* view, program_cache const& program_cache,
    d3d9::graphics_device& device)
{
    if (calls.empty())
        return;

    std::vector<variable_owner const*> owners;
    owners.reserve(8);
    owners.push_back(&view->variables());

    for (draw_call const& draw_call : calls)
    {
        drawable const& drawable = *draw_call.drawable;
        surface_variables_instance const& surface_variables_instance = *draw_call.surface_variables_instance;

        std::vector<variable_owner const*> new_owners;
        new_owners.assign(owners.begin(), owners.end());
        new_owners.push_back(&surface_variables_instance.variables());
        new_owners.push_back(draw_call.variables);

        shared_shading_program program = program_cache.depth_program_for(
            *surface_variables_instance.shader());

        program->prepare(new_owners, time);
        program->use();

        DWORD culling_state = D3DCULL_CCW;
        if (surface_variables_instance.shader()->is_alpha_tested())
        {
            culling_state = culling_mode(*surface_variables_instance.shader(), *view);
            //culling_state = surface_variables_instance.shader()->d3d_cullmode();
        }

        d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, culling_state);

        drawable.draw();
    }
}

namespace {
    bool has_same_program(draw_call const& a, draw_call const& b)
    {
        return a.surface_variables_instance == b.surface_variables_instance;
            //&& a.transformer == b.transformer;
    }
    // Global dummy light shader.
    std::unique_ptr<shaders::dummy_light_shader> g_unique_dummy_light_shader;
} // anonymous namespace

shaders::light_shader const& global_dummy_light_shader()
{
    if (!g_unique_dummy_light_shader)
        g_unique_dummy_light_shader.reset(new shaders::dummy_light_shader);
    return *g_unique_dummy_light_shader;
}

void render(std::vector<draw_call> const& calls, float time, light* light,
    view* view, program_cache const& program_cache,
    d3d9::graphics_device& device,
    bool usePreshader)
{
    if (calls.empty())
        return;

    std::vector<variable_owner const*> owners;
    owners.reserve(8);

    if (light)
    {
        owners.push_back(&light->variables());
        owners.push_back(&light->light_properties().variables());
    }
    owners.push_back(&view->variables());

    for (draw_call const& draw_call : calls)
    {
        drawable const& drawable = *draw_call.drawable;
        surface_variables_instance const& surface_variables_instance = *draw_call.surface_variables_instance;

        std::vector<variable_owner const*> new_owners;
        new_owners.assign(owners.begin(), owners.end());
        new_owners.push_back(&surface_variables_instance.variables());
        new_owners.push_back(draw_call.variables);

        auto const& light_shader = light ? *light->shader() : global_dummy_light_shader();

        shared_shading_program program;

        //if (usePreshader)
        //    return;

        if (!usePreshader)
        {
            program = program_cache.program_for(
                *surface_variables_instance.shader(),
                light_shader);

            program->prepare(new_owners, time);
            program->use();

            auto culling_state = culling_mode(*surface_variables_instance.shader(), *view);
            d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, culling_state);

            //auto const& planes = view->clipping_planes();
            //DWORD clipping_plane_flags = 0;
            //for (size_t i = 0; i < planes.size(); ++i)
            //{
            ////    clipping_plane_flags |= (1u << i);

            //    auto plane_in_world = planes[i].value_at(time);

            ////    view->projection().update(time);
            ////    auto projection = view->projection().get();
            ////    auto plane_in_view_clip = transform(plane_in_view, projection);

            ////    math::plane left_plane_in_view_clip = math::make_plane(vector3f(-0.8f, 0, 10), vector3f(1, 0, 0));
            ////    math::plane bottom_plane_in_view_clip = math::make_plane(vector3f(0, -0.8f, 10), vector3f(0, 1, 0));

            ////    set_clip_plane(device, i, bottom_plane_in_view_clip.data());
            ////    //set_clip_plane(device, i, plane_in_view_clip.data());
            //}
            ////d3d9::scoped_render_state clipping(device, D3DRS_CLIPPLANEENABLE, clipping_plane_flags);

            drawable.draw();
        }
        else
        {
            if (!surface_variables_instance.pre_shader())
                continue;

            program = program_cache.program_for(
                *surface_variables_instance.pre_shader(),
                light_shader);

            program->prepare(new_owners, time);
            program->use();

            auto culling_state = culling_mode(*surface_variables_instance.shader(), *view);
            d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, culling_state);

            drawable.draw();
        }

    }
}

void render_to_shadowmap(std::vector<draw_call> const& calls, float time,
	view* view, std::shared_ptr<shading_program> program) 
{
	if (calls.empty())
		return;

	std::vector<d3d9::rendering::variable_owner*> owners;
	owners.reserve(8);
	owners.push_back(&view->variables());

    for (draw_call const& draw_call : calls)
	{
        drawable const& drawable = *draw_call.drawable;

		std::vector<variable_owner const*> new_owners;		
		new_owners.assign(owners.begin(), owners.end());
        new_owners.push_back(draw_call.variables);
		
		program->prepare(new_owners, time);
		program->use();

        //auto culling_state = culling_mode(*surface_variables_instance.shader(), *view);
        //d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, culling_state);

        drawable.draw();
	}
}

void render_transparents(
    std::vector<draw_call> const& calls,
    float time,
    view* view,
    program_cache const& program_cache,
    item** items_with_light,
    size_t light_count,
    graphics_device& device)
{
    if (calls.empty())
        return;

    d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, TRUE);
    d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    for (draw_call const& draw_call : calls)
    {
        drawable const& drawable = *draw_call.drawable;
        surface_variables_instance const& surface_variables_instance = *draw_call.surface_variables_instance;

        bool is_additive
            = surface_variables_instance.shader()->is_additive_blended();
        bool is_transparent
            = surface_variables_instance.shader()->is_transparent();

        std::vector<variable_owner const*> owners;
        owners.reserve(8);
        owners.push_back(&view->variables());

        if (is_additive)
        {
            d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_ONE);

            std::vector<variable_owner const*> new_owners;
            new_owners.assign(owners.begin(), owners.end());
            new_owners.push_back(&surface_variables_instance.variables());
            new_owners.push_back(draw_call.variables);

            auto const& light_shader = global_dummy_light_shader();

            shared_shading_program program = program_cache.program_for(
                *surface_variables_instance.shader(),
                light_shader);

            program->prepare(new_owners, time);
            program->use();

            auto culling_state = culling_mode(*surface_variables_instance.shader(), *view);
            d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, culling_state);

            drawable.draw();
        }
        else if (is_transparent)
        {
            d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

            owners.push_back(&surface_variables_instance.variables());
            owners.push_back(draw_call.variables);

            // Set culling mode for this drawable.
            //int cullmode_flag = surface_variables_instance.shader()->d3d_cullmode();
            //DWORD cullmode = (DWORD)cullmode_flag;
            //d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, cullmode);

            auto culling_state = culling_mode(*surface_variables_instance.shader(), *view);
            d3d9::scoped_render_state culling(device, D3DRS_CULLMODE, culling_state);

            d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, false);

            //d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            //d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

            for (size_t light_index = 0; light_index < light_count; ++light_index)
            {
                d3d9::rendering::light* light = &items_with_light[light_index]->light();
                auto const& light_shader = light ? *light->shader() : global_dummy_light_shader();

                shared_shading_program program = program_cache.program_for(*surface_variables_instance.shader(), light_shader);

                std::vector<variable_owner const*> per_light_owners;
                per_light_owners.assign(owners.begin(), owners.end());
                per_light_owners.push_back(&light->variables());
                per_light_owners.push_back(&light->light_properties().variables());

                // Set up program and draw

                program->prepare(per_light_owners, time);
                program->use();

                drawable.draw();
            }
        }
        else
        {
            assert(false && "not additive nor transparent");
        }
    }

    d3d9::set_render_state(device, D3DRS_ALPHABLENDENABLE, FALSE);
    d3d9::set_render_state(device, D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
    d3d9::set_render_state(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
    d3d9::set_render_state(device, D3DRS_ZWRITEENABLE, true);
}

void update(surface& surface,
    item* item_with_light,
    program_cache& program_cache,
    std::vector<variable_owner*> const& owners)
{
    auto const& light_shader = item_with_light ? *item_with_light->light().shader() : global_dummy_light_shader();

    // TODO: Loop every light (and its light_shader) with all surfaces (and
    // its surface_shader) and create all resources for all combinations.

    for (surface_variables_instance& surface_variables_instance : surface.instances())
    {
        //Create material pre shader
        if (surface_variables_instance.pre_shader())
        {
            shared_shading_program program
                = program_cache.find_or_create(*surface_variables_instance.pre_shader(), global_dummy_light_shader());

            std::vector<variable_owner*> new_owners = owners;
            new_owners.push_back(&surface_variables_instance.pre_shader_variables());

            program->create_resources(new_owners);
        }

        //Create material / surface shader
        {
            shared_shading_program program
                = program_cache.find_or_create(*surface_variables_instance.shader(), light_shader);

            std::vector<variable_owner*> new_owners = owners;
            new_owners.push_back(&surface_variables_instance.variables());

            program->create_resources(new_owners);
        }

        //Create depth shader
        {
            shared_shading_program program
                = program_cache.find_or_create_depth(*surface_variables_instance.shader());

            std::vector<variable_owner*> new_owners = owners;
            new_owners.push_back(&surface_variables_instance.variables());

            program->create_resources(new_owners);
        }
    }
}

void update_shadow_depth(surface& surface,
	std::shared_ptr<d3d9::rendering::shading_program> shadow_depth_program,
	std::vector<variable_owner*> const& shadow_depth_owners)
{
    for (auto& surface_variables_instance : surface.instances())
    {
        //Create shadowmap depth shader
        {
            std::vector<variable_owner*> new_owners = shadow_depth_owners;
            new_owners.push_back(&surface_variables_instance.variables());

            shadow_depth_program->create_resources(new_owners);
        }
    }
}

}}} // namespace pt::d3d9::rendering
