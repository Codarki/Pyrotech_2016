#include "item_builder.h"
#include "item_geometry.h"
#include "item_name.h"
#include "angle_axis_rotation.h"
#include "geometry_builder.h"
#include "camera.h"
#include "camera_builder.h"
#include "composite_transformation.h"
#include "look_at_transformation.h"
#include "material_cache.h"
#include "light.h"
#include "light_builder.h"
#include "rotation.h"
#include "simple_transformation.h"
#include "static_vertex_stream.h"
#include "srt_transformation.h"
#include "translation.h"
#include "transformer.h"
#include "transformer_builder.h"
#include "source/vertex_attribute.h"
#include "../stream_source.h"
#include "pt/animatable_value_utility.h"
#include "pt/format.h"
#include "pt/log.h"
#include "pt/parameter_node.h"
#include "pt/vector_utility.h"
#include "pt/matrix_operations.h"
#include "pt/math_utils.h"

#include "pt/math/kb_spline.h"
#include "pt/math/spherical_cubic_interpolation.h"

#include "p3d_format/animations.h"
#include "p3d_format/camera.h"
#include "p3d_format/light.h"
#include "p3d_format/mesh.h"
#include "p3d_format/readers.h"
#include "p3d_format/scene.h"
#include "p3d_format/triangular_mesh.h"

#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    using animatable_float = animatable_value<float>;
    using animatable_vector3f = animatable_value<vector3f>;
    using animatable_vector4f = animatable_value<vector4f>;

    using animatable_float_source = animatable_value_source<float>;
    using animatable_vector3f_source = animatable_value_source<vector3f>;
    using animatable_vector4f_source = animatable_value_source<vector4f>;

    using shared_animatable_float_source = std::shared_ptr<animatable_value_source<float>>;
    using shared_animatable_vector3f_source = std::shared_ptr<animatable_value_source<vector3f>>;
    using shared_animatable_vector4f_source = std::shared_ptr<animatable_value_source<vector4f>>;

    using constant_float_source = constant_value_source<float>;
    using constant_vector3f_source = constant_value_source<vector3f>;
    using constant_vector4f_source = constant_value_source<vector4f>;

    shared_animatable_float_source create_float_source(
        math::kb_data_set<float> const& data, float default_value)
    {
        if (data.size() == 0)
            return std::make_shared<constant_float_source>(default_value);
        else if (data.size() == 1)
            return std::make_shared<constant_float_source>(data.at(0).value());

        math::kb_splinef spline(data);
        return std::make_shared<math::kb_splinef_value_source>(spline);
    }

    parameter_collection create_mesh_parameters(
        p3d_format::item const& mesh)
    {
        auto name = std::make_shared<parameter_node>(
            "name", create_constant_value(mesh.name()));

        parameter_collection result;
        result.add(name);

        math::kb_data_set<float> const& data = mesh.animation()->hide_data();
        float default_value = 1; //1 - visible, 0 - hidden

        auto hide_source_value = create_float_source(data, default_value);
        auto hide_value = std::make_shared<animatable_float>(
            hide_source_value);

        auto visibility = std::make_shared<parameter_node>("visibility", hide_value);
        result.add(visibility);

        return result;
    }

    parameter_collection create_light_parameters(
        p3d_format::light const& light,
        config_node light_config)
    {
        // TODO: animatables?
        auto name = std::make_shared<parameter_node>(
            "name", create_constant_value(light.name()));
        auto position = std::make_shared<parameter_node>(
            "position", create_constant_value(light.position()));
        auto target = std::make_shared<parameter_node>(
            "target", create_constant_value(light.target()));
        auto color = std::make_shared<parameter_node>(
            "color", create_constant_value(light.color()));
        auto is_spot = std::make_shared<parameter_node>(
            "is_spot", create_constant_value(light.is_spot()));
        auto hotspot = std::make_shared<parameter_node>(
            "hotspot", create_constant_value(light.hotspot()));
        auto falloff = std::make_shared<parameter_node>(
            "falloff", create_constant_value(light.falloff()));
        auto range = std::make_shared<parameter_node>(
            "range", create_constant_value(light.range()));
        auto multiplier = std::make_shared<parameter_node>(
            "multiplier", create_constant_value(light.multiplier()));

        parameter_collection result;
        result.add(name);
        result.add(position);
		result.add(target);
        result.add(color);
        result.add(is_spot);
        result.add(hotspot);
        result.add(falloff);
        result.add(range);
        result.add(multiplier);

        //math::kb_data_set<float> const& data = light.animation()->fov_data();
        math::kb_data_set<float> const& data = light.animation()->falloff_data();
        auto anitmatable_falloff_source_value = create_float_source(data, light.falloff());
        auto anitmatable_falloff_value = std::make_shared<animatable_float>(anitmatable_falloff_source_value);
        auto anitmatable_falloff = std::make_shared<parameter_node>("animatable_falloff", anitmatable_falloff_value);
        result.add(anitmatable_falloff);

        float near_plane = 1.0f;
        float far_plane = 200.0f;
        bool is_casting_shadows = false;
        float shadow_bias = 0.00011f;
        if (light_config.key() == "light")
        {
            //std::shared_ptr<parameter_node> result = create("light material", m_name);

            std::string light_shader_instance = light_config.child("light_shader_instance")->child("name")->value();
            auto light_shader_instance_name = std::make_shared<parameter_node>(
                "light_shader_instance_name", create_constant_value(light_shader_instance));
            result.add(light_shader_instance_name);

            light_config.parse_optional_child("near_plane", near_plane);
            light_config.parse_optional_child("far_plane", far_plane);

            light_config.parse_optional_child("cast_shadows", is_casting_shadows);
            light_config.parse_optional_child("shadow_bias", shadow_bias);
            
            // TODO: <lights><light> children:
            // light_shader_instance
            // field_of_view
            // shadow_mapped
        }
        else
        {
            std::string light_shader_instance = "default_point_light";
            if (light.is_spot())
                light_shader_instance = "default_frustum_light";
            auto light_shader_instance_name = std::make_shared<parameter_node>(
                "light_shader_instance_name", create_constant_value(light_shader_instance));
            result.add(light_shader_instance_name);
        }

        auto near_plane_node = std::make_shared<parameter_node>(
            "near_plane", create_constant_value(near_plane));
        auto far_plane_node = std::make_shared<parameter_node>(
            "far_plane", create_constant_value(far_plane));
        auto is_casting_shadows_node = std::make_shared<parameter_node>(
            "cast_shadows", create_constant_value(is_casting_shadows));
        auto shadow_bias_node = std::make_shared<parameter_node>(
            "shadow_bias", create_constant_value(shadow_bias));

        result.add(near_plane_node);
        result.add(far_plane_node);
        result.add(is_casting_shadows_node);  
        result.add(shadow_bias_node);

        return result;
    }

    animatable_vector4f create_animatable_vector4f(
        math::data_set<float> const* data, vector4f const& default_value)
    {
        if (data[0].size() == 0)
        {
            auto source = std::make_shared<constant_vector4f_source>(
                default_value);
            return animatable_vector4f(source);
        }

        if (data[0].size() > 1)
            log("rotation tracks not implemented yet, discarding");

        //if (data[0].size() == 1)
        //{
            vector4f value;
            value[0] = data[0].begin()->value();
            value[1] = data[1].begin()->value();
            value[2] = data[2].begin()->value();
            value[3] = data[3].begin()->value();

            auto source = std::make_shared<constant_vector4f_source>(
                value);

            return animatable_vector4f(source);
        //}

        //assert(false);
        //return animatable_value<vector4f>();
    }

    angle_axis_rotation create_angle_axis_rotation(
        math::data_set<float> const* data, vector4f const& default_value)
    {
        if (data[0].size() == 0)
        {
            auto angle_source = std::make_shared<constant_float_source>(
                default_value[0]);

            vector3f default_axis(default_value[1], default_value[2],
                default_value[3]);

            auto axis_source = std::make_shared<constant_vector3f_source>(
                default_axis);

            animatable_float angle_value(angle_source);
            animatable_vector3f axis_value(axis_source);

            return angle_axis_rotation(angle_value, axis_value);
        }

        if (data[0].size() > 1)
            log("rotation tracks not implemented yet, discarding");

        //if (data[0].size() == 1)
        //{
            vector4f value;
            value[0] = data[0].begin()->value();
            value[1] = data[1].begin()->value();
            value[2] = data[2].begin()->value();
            value[3] = data[3].begin()->value();

            auto angle_source = std::make_shared<constant_value_source<float>>(
                value[0]);

            auto axis_source = std::make_shared<constant_value_source<vector3f>>(
                vector3f(value[1], value[2], value[3]));

            animatable_float angle_value(angle_source);
            animatable_vector3f axis_value(axis_source);

            return angle_axis_rotation(angle_value, axis_value);
        //}

        //assert(false);
        //return animatable_value<vector4f>();

        //return angle_axis_rotation(create_vector4f_value(
        //    item.animation()->rotation_data(), vector4f(0, 0, 0, 0)));
    }

    boost::math::quaternion<float> axis_angle_to_quaternion(vector4f const& axis_angle)
    {
        float angle = axis_angle[0];
        D3DXVECTOR3 axis(axis_angle[1], axis_angle[2], axis_angle[3]);
        D3DXVec3Normalize(&axis, &axis);

        D3DXQUATERNION q;
        D3DXQuaternionIdentity(&q);

        D3DXQuaternionRotationAxis(&q, &axis, angle);

        D3DXQUATERNION q2;
        D3DXQuaternionNormalize(&q2, &q);

        return math::quaternionf(q2.w, q2.x, q2.y, q2.z);
    }

    void create_quaternion_rotation(
        math::kb_data_set<float> const axis_angles[4],
        math::kb_data_set<boost::math::quaternion<float> >& quaternions)
    {
        assert(quaternions.size() == 0);
        
        // Create quaternion with relative rotation.
        size_t count = axis_angles[0].size();
        for (size_t i = 0; i < count; ++i)
        {
            vector4f axis_angle(
                axis_angles[0].at(i).value(),
                axis_angles[1].at(i).value(),
                axis_angles[2].at(i).value(),
                axis_angles[3].at(i).value());
            
            float time = axis_angles[0].at(i).time();
            assert(axis_angles[1].at(i).time() == time);
            assert(axis_angles[2].at(i).time() == time);
            assert(axis_angles[3].at(i).time() == time);

            typedef math::kb_data_set<float>::point kb_aa_point;
            typedef math::kb_data_set<boost::math::quaternion<float> >::point kb_point;
            kb_aa_point const axis_angle_point = axis_angles[0].at(i);

            kb_point new_point(time,
                axis_angle_to_quaternion(axis_angle),
                axis_angle_point.parameters(),
                math::quaternionf(),
                math::quaternionf());

            quaternions.add(new_point);
        }

        // TODO KimmoP 13.02.2009: This comment disables absolute rotations!
        for (size_t i = 1; i < count; ++i)
        {
            typedef math::kb_data_set<boost::math::quaternion<float> >::point kb_point;
            kb_point const current_point = quaternions.at(i);

            D3DXQUATERNION q_prev(
                quaternions.at(i - 1).value().R_component_2(),
                quaternions.at(i - 1).value().R_component_3(),
                quaternions.at(i - 1).value().R_component_4(),
                quaternions.at(i - 1).value().R_component_1());

            D3DXQUATERNION q(
                quaternions.at(i).value().R_component_2(),
                quaternions.at(i).value().R_component_3(),
                quaternions.at(i).value().R_component_4(),
                quaternions.at(i).value().R_component_1());

            D3DXQuaternionNormalize(&q_prev, &q_prev);
            D3DXQuaternionNormalize(&q, &q);

            //float angle = D3DXQuaternionDot(&q_prev, &q);
            //if (angle < -0.0f)
            //{
            //    q = -q;
            //}

            D3DXQUATERNION out;
            D3DXQuaternionMultiply(&out, &q_prev, &q);
            //D3DXQuaternionMultiply(&out, &q, &q_prev);
            

            D3DXQuaternionNormalize(&out, &out);

            kb_point new_point(current_point.time(),
                math::quaternionf(out.w, out.x, out.y, out.z),
                current_point.parameters(),
                current_point.starting_tangent(),
                current_point.ending_tangent());

            quaternions.at(i) = new_point;
        }
    }


    rotation create_quaternion_rotation(
        math::kb_data_set<float> const data[4],
        vector4f const& default_value)
    {
        if (data[0].size() == 0)
        {
            math::quaternionf q(
                default_value[0], default_value[1],
                default_value[2], default_value[3]);

            auto source = std::make_shared<constant_value_source<math::quaternionf>>(q);

            return rotation(animatable_value<math::quaternionf>(source));
        }
        else if (data[0].size() == 1 && data[1].size() == 1 && data[2].size() == 1 && data[3].size() == 1)
        {
            vector4f axis_angle(
                data[0].at(0).value(),
                data[1].at(0).value(),
                data[2].at(0).value(),
                data[3].at(0).value());

            auto q = axis_angle_to_quaternion(axis_angle);

            auto source = std::make_shared<constant_value_source<math::quaternionf>>(q);

            return rotation(animatable_value<math::quaternionf>(source));
        }

        math::kb_data_set<boost::math::quaternion<float> > quaternions;
        create_quaternion_rotation(data, quaternions);

        //std::vector<math::kochanek_bartels_parameters> parameters[4];
        //for (size_t i = 0; i < 4; ++i)
        //{
        //    size_t count = data[i].size();
        //    for (size_t j = 0; j < count; ++j)
        //    {
        //        parameters[i].push_back(
        //            math::kochanek_bartels_parameters(0.0f, 0, 0));
        //    }
        //}

        //math::kb_data_set<boost::math::quaternion<float> > spline_data(
        //    quaternions, parameters[0]);

        // TODO wrong dataset!
        auto spline = math::spherical_cubic_interpolation(quaternions);
        auto source = std::make_shared<math::squad_value_source>(spline);

        return rotation(
            animatable_value<boost::math::quaternion<float> >(source));
    }


    animatable_vector3f create_animatable_vector3f(
        math::kb_data_set<float> const* data, vector3f const& default_value)
    {
        if (data[0].size() == 0)
        {
            return animatable_vector3f(
                std::make_shared<constant_vector3f_source>(default_value));
        }
        else if (data[0].size() == 1 && data[1].size() == 1 && data[2].size() == 1)
        {
            vector3f default_value2(
                data[0].at(0).value(),
                data[1].at(0).value(),
                data[2].at(0).value());

            return animatable_vector3f(
                std::make_shared<constant_vector3f_source>(default_value2));
        }

        assert(data[1].size() > 0);
        assert(data[2].size() > 0);

        math::kb_spline3f spline(data);
        auto source = std::make_shared<math::kb_spline3f_value_source>(spline);
        return animatable_vector3f(source);
    }

	std::shared_ptr<transformation> create_translation(
		boost::shared_ptr<p3d_format::animation const> animation)
    {
        auto value = create_animatable_vector3f(
            animation->position_data(), vector3f(0, 0, 0));

        return std::make_shared<translation>(value);
    }

    std::shared_ptr<transformation> default_transformation(
        matrix4x4f const& default_pose)
    {
        return std::make_shared<simple_transformation>(default_pose);
    }

    std::shared_ptr<transformation> optional_transformation(
        p3d_format::item const& item)
    {
		auto animation = create_translation(item.animation());

        if (item.target_animation())
        {
			auto target_animation
                = create_translation(item.target_animation());

            auto lookat = std::make_shared<look_at_transformation>(
                animation, target_animation);

            math::kb_data_set<float> const& roll_data = item.animation()->roll_data();
            size_t roll_count = roll_data.size();
            if (roll_count > 0)
            {
                float default_roll = roll_data.at(0).value();
                std::shared_ptr<animatable_value_source<float> > animatable_roll_source_value;

                if (roll_count > 1)
                {
                    animatable_roll_source_value = create_float_source(roll_data, default_roll);
                }
                else
                {
                    animatable_roll_source_value = std::make_shared<constant_float_source>(default_roll);
                }

                animatable_float angle(animatable_roll_source_value);

                animatable_vector3f axis(
                    std::make_shared<constant_vector3f_source>(vector3f(0, 0, 1)));

                auto roll = std::make_shared<angle_axis_rotation>(angle, axis);

                auto composite = std::make_shared<composite_transformation>();
                composite->push_back(roll);
                composite->push_back(lookat);

                return composite;
            }

            return lookat;
        }
        else
        {
            rotation rotation = create_quaternion_rotation(
                item.animation()->rotation_data(), vector4f(1, 0, 0, 0));

            translation translation(create_animatable_vector3f(
                item.animation()->position_data(), vector3f(0, 0, 0)));

            scaling scale(create_animatable_vector3f(
                item.animation()->scaling_data(), vector3f(1, 1, 1)));
            
            vector3f pivot = item.animation()->pivot();

            if (within_denormal(pivot))
            {
                return std::make_shared<srt_transformation>(
                    scale, rotation, translation);
            }
            else
            {
                log(format("Pivot: %1%") % to_string(pivot));

                return std::make_shared<pivoted_srt_transformation>(
                    scale, rotation, translation, pivot);
            }
        }
    }
} // anonymous namespace

item_builder::item_builder(graphics_device& device,
    parameter_collection const& parameters, progress_report* progress)
: m_device(&device)
, m_parameters(&parameters)
, m_progress(progress)
{
}

void item_builder::build(path const& path, material_cache& materials,
    topology_cache& topologies)
{
    log(format("Loading mesh model from file '%1%'") % path.string());
    //logging::scoped_indent indent;

	auto mesh = p3d_format::read_mesh(path, m_progress);
    build(*mesh, materials, topologies);
}

void item_builder::build(p3d_format::camera const& camera, std::shared_ptr<pt::d3d9::rendering::projection_matrix> projection)
{
    log(format("Building camera item '%1%'") % camera.name());
    logging::scoped_indent indent;

    auto default_pose = translation_matrix(camera.position());

    transformer_builder transformer_builder(
        optional_transformation(camera),
        default_transformation(default_pose));

    auto transformer = transformer_builder.unique_transformer();
    auto parameters = create_camera_parameters(camera);

    std::vector<animatable_value<math::plane>> clipping_planes;
    
    camera_builder builder(transformation_from(*transformer), parameters, false);
    if (projection != nullptr)
        builder = camera_builder(transformation_from(*transformer), projection, clipping_planes, false);

    // TODO: Same parameters as with camera?
    m_item = std::make_shared<item>(camera.name(), move(transformer));
    m_item->set_camera(builder.unique_camera());
}

void item_builder::build(p3d_format::light const& light, config_node light_config, material_cache& materials)
{
    log(format("Building light item '%1%'") % light.name());
    logging::scoped_indent indent;

    auto default_pose = translation_matrix(light.position());

    //LIGHT_BUILDER HERE!
    //light_config

    transformer_builder transformer_builder(
        optional_transformation(light),
        default_transformation(default_pose));

    auto parameters = create_light_parameters(light, light_config);
    m_item = std::make_shared<item>(light.name(), transformer_builder.unique_transformer());

    m_item->set_light(create_unique_light(
        transformation_from(m_item->transformer()), parameters, materials, *m_device));
}

void item_builder::build(p3d_format::mesh const& mesh,
    material_cache& materials, topology_cache& topologies)
{
    path vb_path(mesh.filename());
    vb_path.replace_extension(".geometry");

    geometry_builder geometry_builder(*m_device);
    geometry_builder.set_source(mesh);
    geometry_builder.set_parameters(*m_parameters);
    geometry_builder.set_material_cache(materials);
    geometry_builder.set_topology_cache(topologies);
    geometry_builder.set_vertex_stream_filename(vb_path.string());

    // TODO create mesh_builder?
    transformer_builder transformer_builder(
        optional_transformation(mesh),
        default_transformation(mesh.default_pose()));

    //Mesh parameters, visibility animation track is kept here
    //auto parameters = create_mesh_parameters(mesh);

    // Create and set up item.
    m_item = std::make_shared<item>(
        mesh.name(),
        transformer_builder.unique_transformer());

    auto unique_item_geometry = build_unique_item_geometry(
        geometry_builder.shared_geometry(),
        transformation_from(m_item->transformer()));

    m_item->set_geometry(move(unique_item_geometry));
    
    //log(format("visibility = %1%") % m_item->parameters().find_child("visibility")->get()->name());
}

void item_builder::build(p3d_format::serialized_mesh const& mesh,
    material_cache& materials)
{
    path vb_path(mesh.filename());
    vb_path.replace_extension(".geometry");

    geometry_builder geometry_builder(*m_device);
    geometry_builder.set_serialized_source(mesh);
    geometry_builder.set_parameters(*m_parameters);
    geometry_builder.set_material_cache(materials);
    //geometry_builder.set_topology_cache(topologies);
    geometry_builder.set_vertex_stream_filename(vb_path.string());

    // TODO create mesh_builder?
    transformer_builder transformer_builder(
        optional_transformation(mesh),
        default_transformation(mesh.default_pose()));

    //Mesh parameters, visibility animation track is kept here
    //auto parameters = create_mesh_parameters(mesh);

    // Create and set up item.
    m_item = std::make_shared<item>(
        mesh.name(),
        transformer_builder.unique_transformer());

    auto unique_item_geometry = build_unique_item_geometry(
        geometry_builder.shared_geometry(),
        transformation_from(m_item->transformer()));

    m_item->set_geometry(move(unique_item_geometry));
    
    //log(format("visibility = %1%") % m_item->parameters().find_child("visibility")->get()->name());
}

std::shared_ptr<item> item_builder::built_item() const
{
    return m_item;
}

parameter_collection create_camera_parameters(
    p3d_format::camera const& camera)
{
    auto name = std::make_shared<parameter_node>(
        "name", create_constant_value(camera.name()));

    parameter_collection result;
    result.add(name);

    math::kb_data_set<float> const& data = camera.animation()->fov_data();
    auto fov_source_value = create_float_source(data, camera.lens());
    auto fov_value = std::make_shared<animatable_float>(fov_source_value);
    auto fov = std::make_shared<parameter_node>("fov", fov_value);
    result.add(fov);

    return result;
}

}}} // namespace pt::d3d9::rendering
