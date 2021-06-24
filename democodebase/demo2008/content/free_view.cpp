#include "free_view.h"
#include "pt/matrix_operations.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/d3d9/formatted_text_source.h"
#include "pt/d3d9/utility.h"
#include "pt/d3d9/rendering/projection_matrix.h"
#include "pt/d3d9/rendering/constant_setter_utility.h"
#include "pt/d3d9/rendering/simple_variable_owner.h"
//#include "pt/d3d9/rendering/srt_transformation.h"
#include "pt/d3d9/rendering/simple_transformation.h"
#include "pt/ui/common.h"
#include "pt/ui/mouse_event.h"

#include "pt/animatable_value.h"
#include "pt/parameter_node.h"

namespace content {

namespace {
    class view_variables : public d3d9::rendering::simple_variable_owner
    {
    public:
        view_variables(matrix4x4f const& local_to_world,
            matrix4x4f const& projection,
            matrix4x4f const& world_to_local_clip,
            vector3f const& view_in_world)
        :   m_local_to_world(local_to_world)
        ,   m_projection(projection)
        ,   m_world_to_local_clip(world_to_local_clip)
        ,   m_view_in_world(view_in_world)
        {
        }
    protected: // interface required by d3d9::rendering::simple_variable_owner
        std::unique_ptr<d3d9::rendering::constant_setter> on_create_constant(
            d3d9::constant const& constant) const
        {
            if (constant.name() == "projection")
            {
                return d3d9::rendering::create_unique_reference_setter<matrix4x4f>(
                    m_projection, constant);
            }
            else if (constant.name() == "projection")
            {
                return d3d9::rendering::create_unique_reference_setter<matrix4x4f>(
                    m_projection, constant);
            }
            else if (constant.name() == "world_to_view_clip")
            {
                return d3d9::rendering::create_unique_reference_setter<matrix4x4f>(
                    m_world_to_local_clip, constant);
            }
            else if (constant.name() == "view_in_world")
            {
                return d3d9::rendering::create_unique_reference_setter<vector3f>(
                    m_view_in_world, constant);
            }

            return nullptr;
        }
    private: // data members
        matrix4x4f const& m_local_to_world;
        matrix4x4f const& m_projection;
        matrix4x4f const& m_world_to_local_clip;
        vector3f const& m_view_in_world;
    };
} // anonymous namespace

// free_view_controller

free_view_controller::free_view_controller()
:   m_pitch(0)
,   m_heading(0)
,   m_last_update_time(-1.0f)
,   m_dragging_tool(0, 0)
{
    // Create animatable sources.
    m_fov.reset(new basic_value_source<float>(45.0f));
    //m_scale.reset(new basic_value_source<vector3f>(vector3f(1, 1, 1)));
    m_view_position_in_world.reset(
        new basic_value_source<vector3f>(vector3f()));

    std::shared_ptr<animatable_value<float> > animatable_fov(
        new animatable_value<float>(m_fov));

    m_projection.reset(
        new d3d9::rendering::projection_matrix(animatable_fov));

    // Create parameters.
    //m_parameters.add(make_shared_ptr(
    //    new parameter_node("fov", animatable_fov)));

    // Create transformation.
    //animatable_value<vector3f> animatable_scale(m_scale);
    //animatable_value<vector3f> animatable_translation(
    //    m_view_position_in_world);

    //d3d9::rendering::scaling scalee(animatable_scale);
    //d3d9::rendering::translation translation(animatable_translation);

    //m_transformation.reset(new d3d9::rendering::srt_transformation(
    //    scalee, d3d9::rendering::rotation(), translation, vector3f(0, 0, 0)));

    m_transformation.reset(new d3d9::rendering::simple_transformation());

    // Create info text.
    m_info_text.reset(new d3d9::formatted_text_source<float,7>);

    m_info_text->set(
        boost::format("Free view fov %=3.2f position(%=3.2f %=3.2f %=3.2f) direction(%=3.2f %=3.2f %=3.2f)"));
    m_info_text->set(0, 0.0f);
    m_info_text->set(1, 0.0f);
    m_info_text->set(2, 0.0f);
    m_info_text->set(3, 0.0f);
    m_info_text->set(4, 0.0f);
    m_info_text->set(5, 0.0f);
    m_info_text->set(6, 0.0f);
}

//parameter_collection const& free_view_controller::parameters() const
//{
//    return m_parameters;
//}

std::shared_ptr<d3d9::rendering::transformation>
    free_view_controller::transformation() const
{
    return m_transformation;
}

std::shared_ptr<d3d9::rendering::projection_matrix>
    free_view_controller::projection() const
{
    return m_projection;
}

std::shared_ptr<d3d9::text_source const> free_view_controller::info_text() const
{
    return m_info_text;
}

void free_view_controller::update(float time)
{
    float dt = time - m_last_update_time;
    
    if (dt == 0.0f)
        dt = 0.1f;
        //return;

    if (dt < 0.0f)
    {
        m_last_update_time = time;
        dt = 0.1f;
        //return;
    }

    m_last_update_time = time;

    float speed_scale = 1.0f;
    if (ui::is_key_down(VK_SHIFT))
        speed_scale = 0.33f;

    if (ui::is_key_down(VK_CONTROL))
        speed_scale = 3.0f;

    vector3f position_delta;

    if (ui::is_key_down(VK_LEFT))
        position_delta -= m_side_vector_in_world;
    if (ui::is_key_down(VK_RIGHT))
        position_delta += m_side_vector_in_world;

    if (ui::is_key_down(VK_UP))
        position_delta += m_direction_in_world;
    if (ui::is_key_down(VK_DOWN))
        position_delta -= m_direction_in_world;

    if (ui::is_key_down(VK_PRIOR))
        position_delta[1] += 1;
    if (ui::is_key_down(VK_NEXT))
        position_delta[1] -= 1;

    float movement_speed = 10.0f * dt * speed_scale;
    position_delta *= movement_speed;

    //*m_view_position_in_world += position_delta;

    m_view_position_in_world->set(
        m_view_position_in_world->value_at(0) + position_delta);

    // Fov F+ F- 
    if (ui::is_key_down(VK_ADD) && ui::is_key_down('F'))
    {
        *m_fov += 1.0f;
        //update_projection();
    }
    else if (ui::is_key_down(VK_SUBTRACT) && ui::is_key_down('F'))
    {
        *m_fov -= 1.0f;
        //update_projection();
    }

    float heading = m_heading;
    float pitch = m_pitch;
    if (m_dragging_tool.is_active())
    {
        float mouse_speed = 0.005f;

        heading += m_dragging_tool.delta()[0] * mouse_speed;
        pitch += m_dragging_tool.delta()[1] * mouse_speed;
    }
    update(heading, pitch);
}

void free_view_controller::handle_event(ui::event& event)
{
    m_dragging_tool.handle_event(event);

    float mouse_speed = 0.005f;

    if (m_dragging_tool.is_active())
    {
        float heading = m_heading + m_dragging_tool.delta()[0] * mouse_speed;
        float pitch = m_pitch + m_dragging_tool.delta()[1] * mouse_speed;

        update(heading, pitch);
    }
    else if (m_dragging_tool.is_finished())
    {
        m_heading += m_dragging_tool.delta()[0] * mouse_speed;
        m_pitch += m_dragging_tool.delta()[1] * mouse_speed;

        m_dragging_tool.end_position_and_clear();

        update(m_heading, m_pitch);
    }
}

void free_view_controller::update(float heading, float pitch)
{
    matrix4x4f pt_rot_x = rotation_matrix(vector3f(1,0,0), pitch);
    matrix4x4f pt_rot_y = rotation_matrix(vector3f(0,1,0), heading);

    matrix4x4f rotation_world_to_local = pt_rot_y * pt_rot_x;

    m_direction_in_world = basis_axis_z(rotation_world_to_local);
    m_side_vector_in_world = basis_axis_x(rotation_world_to_local);

    matrix4x4f inv_translation
        = translation_matrix(-m_view_position_in_world->value_at(0.0f));

    matrix4x4f world_to_local = inv_translation * rotation_world_to_local;

    //m_local_to_world = invert(world_to_local);

    m_transformation->set_local_to_parent(invert(world_to_local));
    //m_transformation->set_orientation(invert(world_to_local));

    // TODO thought the order was other way around..hmm
    //m_world_to_local_clip = world_to_local * m_projection;

    vector3f const& view_in_world = *m_view_position_in_world;

    m_info_text->set(0, *m_fov);
    m_info_text->set(1, view_in_world[0]);
    m_info_text->set(2, view_in_world[1]);
    m_info_text->set(3, view_in_world[2]);
    m_info_text->set(4, m_direction_in_world[0]);
    m_info_text->set(5, m_direction_in_world[1]);
    m_info_text->set(6, m_direction_in_world[2]);
}

} // namespace content
