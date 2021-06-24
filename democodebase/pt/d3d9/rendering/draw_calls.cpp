#include "draw_calls.h"
#include "drawable.h"
#include "geometry.h"
#include "item.h"
#include "item_collection.h"
#include "item_geometry.h"
#include "view.h"
#include "transformer.h"

#include "surface.h"
#include "pt/d3d9/shaders/surface_shader.h"
#include "pt/matrix.h"
#include "pt/matrix_operations.h"
#include "pt/parameter_node_utility.h"
#include "pt/std/algorithm.h"

namespace pt { namespace d3d9 { namespace rendering {

draw_call::draw_call(
    rendering::drawable const* drawable,
    rendering::surface_variables_instance const* surface_variables_instance,
    transformation_variables const* variables,
    d3d9::rendering::transformer const* transformer)
: drawable(drawable)
, surface_variables_instance(surface_variables_instance)
, variables(variables)
, transformer(transformer)
{
}

void push_back_draw_calls(d3d9::rendering::item& item,
    std::vector<draw_call>& draw_calls,
    bool add_opaques,
    bool add_additives,
    bool add_translucents, 
    float time)
{
    //float visibility = value_at<float>(time,item.parameters(),"visibility");
    /*shared_ptr<animatable_value<float> const> visibility_anim
        = get_animatable<float>(**(item.parameters().child("visibility")));

    float visibility = visibility_anim->value_at(time);
    */
    /*shared_ptr<animatable_value<float> const> visibility_anim
        = get_animatable<float>(**item.parameters().find_child("visibility"));
    */
    //parameter_node *pnode = item.parameters().find_child("visibility")->get();

    //float visibility = visibility_anim->value_at(time);
    float visibility = 1;
    if (item.has_geometry() && visibility >= 0.5)
    {
        auto& surface = item.geometry().geometry().surface();
        for (auto const& surface_instance : surface.instances())
        {
            //if (drawable)
            {
                assert(surface_instance.shader());

                bool is_additive
                    = surface_instance.shader()->is_additive_blended();
                bool is_transparent
                    = surface_instance.shader()->is_transparent();

                bool add = false;

                if (is_additive && add_additives)
                    add = true;

                if (!is_additive && !is_transparent && add_opaques)
                    add = true;

                if (is_transparent && !is_additive && add_translucents)
                    add = true;

                if (add)
                {
                    //draw_call call;

                    item_geometry const& item_geometry = item.geometry();
                    auto const& geometry = item_geometry.geometry();
                    //call.drawable = &geometry.drawable();
                    //call.variables = &item_geometry.variables();

                    ////call.item_geometry = &item.geometry();
                    //call.transformer = &item.transformer();
                    //draw_calls.push_back(call);

                    draw_calls.push_back(draw_call(
                        &geometry.drawable(),
                        &surface_instance,
                        &item_geometry.variables(),
                        &item.transformer()
                        ));
                }
            }
        }
    }

    for (auto const& shared_item : item.children())
    {
        d3d9::rendering::item& child = *shared_item;

        push_back_draw_calls(child, draw_calls, add_opaques,
            add_additives, add_translucents, time);
    }
}

//std::vector<draw_call> gather_draw_calls(d3d9::rendering::item& root)
//{
//    std::vector<draw_call> result;
//    result.reserve(100);
//    push_back_draw_calls(root, result, true, true);
//    return result;
//}

std::vector<draw_call> gather_opaque_draw_calls(item& root, float time)
{
    std::vector<draw_call> result;
    result.reserve(100);
    push_back_draw_calls(root, result, true, false, false, time);
    return result;
}

std::vector<draw_call> gather_transparent_draw_calls(item& root, float time)
{
    std::vector<draw_call> result;
    result.reserve(100);
    push_back_draw_calls(root, result, false, false, true, time);
    return result;
}

std::vector<draw_call> gather_additive_draw_calls(item& root, float time)
{
    std::vector<draw_call> result;
    result.reserve(100);
    push_back_draw_calls(root, result, false, true, false, time);
    return result;
}

struct draw_call_sorter
{
    bool operator()(draw_call const& a, draw_call const& b) const
    {
        // address comparison
        return a.surface_variables_instance < b.surface_variables_instance;
        //return &a.item_geometry->geometry().drawable().surface() < &b.item_geometry->geometry().drawable().surface();
    }
};

class transparent_draw_call_sorter
{
public:
    transparent_draw_call_sorter(rendering::item const& camera_item, float time)
    : m_camera_item(&camera_item)
    , m_time(time)
    {
        auto view_to_world
            = local_to_world_at(m_camera_item->transformer(), time);

        m_view_in_world = translation(view_to_world);
    }

    bool operator()(draw_call const& a, draw_call const& b) const
    {
        matrix4x4f const a_to_world = local_to_world_at(*a.transformer, m_time);
        matrix4x4f const b_to_world = local_to_world_at(*b.transformer, m_time);

        vector3f const a_in_world = translation(a_to_world);
        vector3f const b_in_world = translation(b_to_world);

        float squared_distance_to_a = pt::squared_distance(a_in_world, m_view_in_world);
        float squared_distance_to_b = pt::squared_distance(b_in_world, m_view_in_world);

        return squared_distance_to_a > squared_distance_to_b;
    }

private: // data members
    rendering::item const* m_camera_item;
    vector3f m_view_in_world;
    float m_time;
};

void sort_draw_calls(std::vector<draw_call>& calls)
{
    draw_call_sorter sorter;
    std::sort(calls.begin(), calls.end(), sorter);
}

void sort_transparent_draw_calls(item const& camera_item, float time,
    std::vector<draw_call>& calls)
{
    transparent_draw_call_sorter sorter(camera_item, time);
    std::sort(calls.begin(), calls.end(), sorter);
}

}}} // namespace pt::d3d9::rendering
