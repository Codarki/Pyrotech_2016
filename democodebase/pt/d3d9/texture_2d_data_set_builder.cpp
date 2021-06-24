#include "texture_2d_data_set_builder.h"
#include "texture_2d_source.h"
#include "texture_2d_collection.h"
#include "texture_2d_builder.h"
#include "pt/path.h"

namespace pt { namespace d3d9 {

//texture_2d_data_set_builder::texture_2d_data_set_builder(
//    pt::math::step_curve_string const& texture_name_curve)
//{
//}
//
//texture_2d_data_set texture_2d_data_set_builder::build() const
//{
//    texture_2d_data_set result;
//
//    return result;
//}

texture_2d_data_set build_texture_2d_data_set(
    pt::math::step_curve_string_value_source const& value_source,
    graphics_device& device)
{
    return build_texture_2d_data_set(
        value_source.step_curve(),
        device);
}

texture_2d_data_set build_texture_2d_data_set(
    pt::math::step_curve_string const& texture_name_step_curve,
    graphics_device& device)
{
    // Create and add texture_2d's to dataset, where each texture_2d is
    // loaded from texture_name_step_curve.

    texture_2d_data_set result;

    pt::math::data_set<std::string> const& dataset = texture_name_step_curve.dataset();

    auto it = dataset.begin();
    auto end_it = dataset.end();

    for (; it != end_it; ++it)
    {
        pt::math::data_set<std::string>::point const& point = *it;
        float const time = point.time();
        std::string const texture_name = point.value();
        pt::path const path(texture_name);

        // Find or load texture
        std::shared_ptr<texture_2d> texture;
        if (texture = default_texture_2d_collection().find(path.string()))
        {
        }
        else
        {
            texture = load_immutable_texture_2d(device, path);
            default_texture_2d_collection().add(path.string(), texture);
        }

        result.add(time, texture);
    }

    return result;
}

}} // namespace pt::d3d9
