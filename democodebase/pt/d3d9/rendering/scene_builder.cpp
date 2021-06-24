#include "scene_builder.h"
#include "transformer_builder.h"
#include "camera.h"
#include "camera_builder.h"
#include "item_builder.h"
#include "item_collection.h"
#include "item_name.h"
#include "light.h"
#include "reflection.h"
#include "scene.h"
#include "reflection_plane_in_world_source.h"
#include "serialized_mesh_not_found_error.h"
#include "transformer.h"
#include "pt/any_error.h"
#include "pt/basic_value_source.h"
#include "pt/format.h"
#include "pt/log.h"
#include "pt/parameter_collection.h"
#include "pt/progress_report.h"
#include "pt/path.h"
#include "pt/vector_utility.h"
#include "pt/boost/cast.h"
#include "p3d_format/material.h"
#include "p3d_format/readers.h"
#include "p3d_format/scene.h"
#include "p3d_format/topology.h"

#include "p3d_format/camera.h"
#include "p3d_format/light.h"
#include "p3d_format/mesh.h"
#include "p3d_format/animations.h"

namespace pt { namespace d3d9 { namespace rendering {

scene_builder::scene_builder(graphics_device& device, path const& path,
    material_cache& materials, progress_report* progress, bool only_serialize,
    composite_curve_description_collection const& /*composite_curves*/)
{
    log(format("Building scene from file: '%1%'") % path.string());
    logging::scoped_indent indent;

    transformer_builder transformer_builder;

    // TODO: composite_curves

    m_scene.reset(new scene(
        path.string(), transformer_builder.unique_transformer()));

	auto source = p3d_format::read_scene(path, progress, only_serialize);

    using animation_from_item_map = std::map<item*, p3d_format::animation const*>;
    animation_from_item_map animation_from_item;

    // Flat list of items. These will get linked to parent-child hierarchy below.
    auto items = std::vector<std::shared_ptr<item> >();

    // Create camera items.
    for (auto const& shared_camera_ptr : source->cameras())
    {
        p3d_format::camera const& source_item = *shared_camera_ptr;
        item_builder builder(device, parameter_collection(), progress);
        builder.build(source_item, nullptr);
		std::shared_ptr<item> built_item = builder.built_item();
        items.push_back(built_item);
        m_scene->add_camera_item(built_item);

        animation_from_item[built_item.get()] = source_item.animation().get();
        
        if (progress)
            progress->advance();
    }

    // Create light items.

    // Get light_parameters.
    // lights_config = <asset><scene><lights> XML element, containing all <light> elements.
    config_node lights_config;
    if (source->config().key() == "scene")
    {
        auto lights_config_iter = source->config().find_child("lights");
        if (lights_config_iter != source->config().end())
        {
            lights_config = *lights_config_iter;
        }
    }

    for (auto const& shared_light_ptr : source->lights())
    {
        p3d_format::light const& source_item = *shared_light_ptr;

        //// DEBUG: SKIP LIGHTS
        //if (!source_item.is_spot())
        //    continue;

        // Find correct <light> element by name.
        float fov = 90.0f;
        config_node light_config;
        if (!lights_config.key().empty())
        {
            assert(lights_config.key() == "lights");
            for (auto const& light_config_iter : lights_config)
            {
                assert(light_config_iter.key() == "light");
                config_node const& light_item_config = *light_config_iter.child("item");
                config_node const& light_item_name = *light_item_config.child("name");
                if (light_item_name.value() == shared_light_ptr->name())
                {
                    light_config = light_config_iter;

                    // Parse some light parameters. TODO: animatable.
                    light_config.parse_optional_child("field_of_view", fov);

                    break;
                }
            }
        }

        // Build item with light.
        item_builder builder(device, parameter_collection(), progress);
        builder.build(source_item, light_config, materials);
		std::shared_ptr<item> built_item = builder.built_item();
        items.push_back(built_item);
        m_scene->add_light_item(built_item);

        animation_from_item[built_item.get()] = source_item.animation().get();

		if (source_item.is_spot())
		{	
            auto projection = built_item->light().projection();

            p3d_format::camera camera_from_light(source_item.position(), source_item.target(), 0, fov);
			camera_from_light.set_name(source_item.name());
			camera_from_light.set_animation(source_item.animation());
			camera_from_light.set_target_animation(source_item.target_animation());
			
			item_builder light_camera_builder(device, parameter_collection(), progress);
            light_camera_builder.build(camera_from_light, projection);
			
            std::shared_ptr<item> built_light_camera_item = light_camera_builder.built_item();
            items.push_back(built_light_camera_item);
            m_scene->add_camera_item(built_light_camera_item);

            animation_from_item[built_light_camera_item.get()] = camera_from_light.animation().get();
			
		}

        if (progress)
            progress->advance();
    }

    // Create material parameters.
    parameter_collection parameters;
    for (auto const& shared_material_ptr : source->materials())
    {
        parameters.add(shared_material_ptr->parameters());
    }

    if (only_serialize)
    {
        // Create serialized mesh model items.
        for (auto const& shared_serialized_mesh_ptr : source->serialized_meshes())
        {
            try
            {
                p3d_format::serialized_mesh const& source_item = *shared_serialized_mesh_ptr;

                item_builder builder(device, parameters, progress);
                builder.build(source_item, materials);
				std::shared_ptr<item> built_item = builder.built_item();
                items.push_back(built_item);
                //m_scene->push_back(builder.built_item());

                animation_from_item[built_item.get()] = source_item.animation().get();

                if (progress)
                    progress->advance();
            }
            catch (serialized_mesh_not_found_error& e)
            {
                log(format("Ignoring serialized mesh: %1%") % e.what());
            }
        }

    }
    else
    {
        // Create mesh model items.
        topology_cache topologies;
        for (auto const& shared_mesh_ptr : source->models())
        {
            p3d_format::mesh const& source_item = *shared_mesh_ptr;

            item_builder builder(device, parameters, progress);
            builder.build(source_item, materials, topologies);
			std::shared_ptr<item> built_item = builder.built_item();
            items.push_back(built_item);
            //m_scene->push_back(builder.built_item());

            animation_from_item[built_item.get()] = source_item.animation().get();

            if (progress)
                progress->advance();
        }
    }

    // Setup item hierarchy.

    for (auto& shared_item_ptr : items)
    {
        rendering::item& item = *shared_item_ptr;
        p3d_format::animation const& animation = *animation_from_item[&item];

        uint16 const parent_position = animation.hierarchy_parent();
        if (parent_position == 0xffff)
        {
            m_scene->push_back(shared_item_ptr);
            continue;
        }

        bool parent_linked = false;

        for (auto& item_animation : animation_from_item)
        {
            rendering::item& parent_item = *(item_animation.first);
            p3d_format::animation const& parent_source_animation = *(item_animation.second);

            if (parent_source_animation.hierarchy_position() != parent_position)
                continue;

            parent_item.push_back(shared_item_ptr);
            parent_linked = true;
        }

        assert(parent_linked);
    }

    // Generate items
    if (source->config().key() == "scene")
    {
        auto config_iter = source->config().find_child("items");
        if (config_iter != source->config().end())
        {
            config_node items_config = *config_iter;

            for (auto const& item_config_iter : items_config)
            {
                assert(item_config_iter.key() == "item");

                std::string item_name;
                item_config_iter.parse_child("name", item_name);

                // TODO: Do something
                //bool is_generated;
                //item_config_iter.parse_optional_child("is_generated", is_generated, false);

                std::vector<animatable_value<math::plane>> clipping_planes;

                // Parse <transformer> element and generate transformation.
                std::shared_ptr<transformation> transformation;
                auto transformer_iter = item_config_iter.find_child("transformer");
                if (transformer_iter != item_config_iter.end())
                {
                    config_node transformer_config = *transformer_iter;

                    for (auto const& transformer_config_iter : transformer_config)
                    {
                        if (transformer_config_iter.key() == "reflection")
                        {
                            std::string other_item_name = transformer_config_iter.child("other_item")->child("name")->value();
                            auto other_item = get_item(other_item_name, *m_scene);

                            vector3f point;
                            transformer_config_iter.child("reflection_plane")->parse_child("point", point);
                            vector3f normal;
                            transformer_config_iter.child("reflection_plane")->parse_child("normal", normal);

                            auto reflection_in_world_source = std::make_shared<basic_value_source<math::plane>>(
                                math::make_plane(point, normal));

                            //std::string reflection_item_name = transformer_config_iter.child("reflection_plane_item")->child("name")->value();
                            //auto reflection_plane_item = get_item(reflection_item_name, *m_scene);
                            //auto reflection_in_world_source = std::make_shared<reflection_plane_in_world_source>(
                            //    shared_transformation_from(reflection_plane_item->transformer()));

                            animatable_value<math::plane> reflection_in_world(reflection_in_world_source);

                            transformation = std::make_shared<reflection>(
                                shared_transformation_from(other_item->transformer()),
                                reflection_in_world);
                                //shared_transformation_from(reflection_plane_item->transformer()));

                            //auto reflection_plane_in_view_source = std::make_shared<plane_to_local_space_source>(
                            //    reflection_in_world, transformation);
                            //animatable_value<math::plane> reflection_plane_in_view(reflection_plane_in_view_source);

                            clipping_planes.push_back(reflection_in_world);
                        }
                        //else if (transformer_config_iter.key() == "jitter")
                        //{
                        //}
                    }
                }

                if (transformation == nullptr)
                    throw pt::any_error(format("Invalid asset config: no transformation for item '%1%'") % item_name);

                rendering::transformer_builder transformer_builder2(
                    transformation, transformation);

                auto transformer = transformer_builder2.unique_transformer();

                //item_builder builder(device, parameter_collection(), progress);
                rendering::item_name name(item_name);
                auto built_item = std::make_shared<item>(name, move(transformer));

                items.push_back(built_item);
                m_scene->add_camera_item(built_item);

                // TODO: parse 'parent_item' set item parent?
                m_scene->push_back(built_item);

                //animation_from_item[built_item.get()] = source_item.animation().get();

                // Parse <camera> element and set camera to built item.
                auto camera_iter = item_config_iter.find_child("camera");
                if (camera_iter != item_config_iter.end())
                {
                    config_node camera_config = *camera_iter;

                    for (auto const& camera_config_iter : camera_config)
                    {
                        if (camera_config_iter.key() == "projection_item")
                        {
                            std::string projection_item_name = camera_config_iter.child("name")->value();
                            auto projection_item = get_item(projection_item_name, *m_scene);

                            if (!projection_item->has_camera())
                                throw pt::any_error(format("projection_item '%1%' has no camera") % projection_item_name);

                            std::shared_ptr<projection_matrix> projection = projection_item->camera().shared_projection();

                            // TODO: hoist out of the loop
                            camera_builder camera_builder(*transformation, projection, clipping_planes, true);
                            built_item->set_camera(camera_builder.unique_camera());
                        }
                    }
                }

                // Build camera
                //auto parameters = create_camera_parameters(camera);

                //camera_builder builder(transformation_from(*transformer), parameters);
                //if (projection != nullptr)
                //    builder = camera_builder(transformation_from(*transformer), projection);

                //// TODO: Same parameters as with camera?
                //m_item = std::make_shared<item>(camera.name(), move(transformer));
                //m_item->set_camera(builder.unique_camera());

            }
        }
    }
}

std::shared_ptr<scene> scene_builder::built_scene() const
{
    return m_scene;
}

}}} // namespace pt::d3d9::rendering
