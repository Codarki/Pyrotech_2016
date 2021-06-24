#ifndef CONTENT_UTILITY_H
#define CONTENT_UTILITY_H

#include "pt/animatable_value_utility.h"
#include "pt/overridable_value_source.h"
#include "pt/parameter_node_utility.h"
#include "pt/parameter_collection.h"
#include "pt/vector_utility.h"
#include "pt/config_node_fwd.h"
#include "pt/d3d9/texture_2d.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/path_fwd.h"

namespace pt {
    template<typename T>
    class animatable_value;
    class parameter_node;
}

namespace pt { namespace d3d9 { namespace rendering {
    class scene;
    class material_cache;
}}}

namespace content {

using namespace pt;

template<typename T>
std::shared_ptr<animatable_value<T> const> create_curve_value(
    config_node const& config, std::shared_ptr<parameter_node> curves);

std::shared_ptr<parameter_node> parse_curve_parameters(path const& path);
void parse_shared_textures(path const& path, parameter_collection& target,
    d3d9::graphics_device& device);

config_node get_post_processing_parameter_config();
std::shared_ptr<d3d9::texture_2d> create_and_add_lockable_texture2d(d3d9::graphics_device& device, std::string name, int width, int height, int bpp);
std::shared_ptr<d3d9::texture_2d> create_and_add_rendertarget_texture2d(d3d9::graphics_device& device, std::string name, int width, int height, int bpp);

/// Sets a box geometry for all light items.
void set_light_boxes(d3d9::graphics_device& device,
    d3d9::rendering::scene& scene,
    d3d9::rendering::material_cache& materials);

// move to .inl
template<typename T>
inline std::shared_ptr<animatable_value<T> const > create_curve_value(
    config_node const& config, std::shared_ptr<parameter_node> curves)
{
    assert(!curves || curves->name() == "curves");
    parameter_collection const& parameters = curves->children();

    std::shared_ptr<animatable_value<T> const> result;

    config_node::const_iterator i = config.find_child("curve");
    if (i != config.end())
    {
        std::shared_ptr<parameter_node> node = *parameters.child(i->value());
        result = get_animatable<T>(*node);
    }
    else
    {
        T value;
        config.parse(value);

        result = create_constant_value(value);
    }

    return result;
}

template<typename T>
inline std::shared_ptr<animatable_value<T> const > create_optional_curve_value(
    config_node const& config, std::string const& child_name,
    std::shared_ptr<parameter_node> curves, T const& default_value)
{
    config_node::const_iterator child_iter = config.find_child(child_name);
    if (child_iter == config.end())
    {
        return create_constant_value(default_value);
    }

    assert(!curves || curves->name() == "curves");
    parameter_collection const& parameters = curves->children();

    std::shared_ptr<animatable_value<T> const> result;

    config_node::const_iterator i = child_iter->find_child("curve");
    if (i != child_iter->end())
    {
        std::shared_ptr<parameter_node> node = *parameters.child(i->value());
        result = get_animatable<T>(*node);
    }
    else
    {
        T value;
        child_iter->parse(value);

        result = create_constant_value(value);
    }

    return result;
}

inline std::shared_ptr<parameter_node> create_basic_node(std::string const& name,
    std::shared_ptr<animatable_value_source<float> > source)
{
    std::shared_ptr<animatable_value<float> > value(
        new animatable_value<float>(source));

    return std::shared_ptr<parameter_node>(new parameter_node(name, value));
}

inline std::shared_ptr<parameter_node> create_basic_node(std::string const& name,
    std::shared_ptr<animatable_value_source<vector3f> > source)
{
    std::shared_ptr<animatable_value<vector3f> > value(
        new animatable_value<vector3f>(source));

    return std::shared_ptr<parameter_node>(new parameter_node(name, value));
}

inline std::shared_ptr<parameter_node> create_basic_node(std::string const& name,
    std::shared_ptr<animatable_value_source<vector4f> > source)
{
    std::shared_ptr<animatable_value<vector4f> > value(
        new animatable_value<vector4f>(source));

    return std::shared_ptr<parameter_node>(new parameter_node(name, value));
}

inline std::shared_ptr<parameter_node> create_basic_node(std::string const& name,
    std::shared_ptr<animatable_value_source<std::string> > source)
{
    std::shared_ptr<animatable_value<std::string> > value(
        new animatable_value<std::string>(source));

    return std::shared_ptr<parameter_node>(new parameter_node(name, value));
}

inline std::shared_ptr<parameter_node> create_basic_node(std::string const& name,
    std::shared_ptr<animatable_value_source<std::shared_ptr<pt::d3d9::texture_2d>>> source)
{
    std::shared_ptr<animatable_value<std::shared_ptr<pt::d3d9::texture_2d>>> value(
        new animatable_value<std::shared_ptr<pt::d3d9::texture_2d>>(source));

    return std::shared_ptr<parameter_node>(new parameter_node(name, value));
}

template<typename T>
std::shared_ptr<overridable_value_source<T> > create_overridable_value_source(
    config_node const& config, std::shared_ptr<parameter_node> curves)
{
    return std::shared_ptr<overridable_value_source<T> >(
        new overridable_value_source<T>(
        create_curve_value<T>(config, curves)));
}

template<typename T>
std::shared_ptr<animatable_value_source<T> > create_animatable_value_source(
    config_node const& config, std::shared_ptr<parameter_node> curves)
{
    std::shared_ptr<animatable_value<T> const > const_value = create_curve_value<T>(config, curves);
    std::shared_ptr<animatable_value<T>> value = std::const_pointer_cast<animatable_value<T>>(const_value);
    return value->source();

    //return shared_ptr<animatable_value_source<T> >();
}

} // namespace content

#endif
