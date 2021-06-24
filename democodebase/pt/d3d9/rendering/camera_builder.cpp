#include "camera_builder.h"
#include "camera.h"
#include "composite_variable_owner.h"
#include "projection_matrix.h"
#include "view_variables.h"
#include "pt/parameter_node_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace {
    composite_variable_owner create_camera_variables(
        transformation const& transformation,
        projection_matrix& projection,
        std::vector<animatable_value<math::plane>> clipping_planes)
    {
        composite_variable_owner result;
        result.push_back(std::make_shared<view_variables>(
            transformation,
            projection,
            clipping_planes));
        return result;
    }
}

// camera_builder

camera_builder::camera_builder(
    transformation const& transformation,
    parameter_collection parameters,
    bool flip_culling_mode)
: m_transformation(&transformation)
, m_parameters(parameters)
, m_flip_culling_mode(flip_culling_mode)
{
    std::shared_ptr<animatable_value<float> const> fov
        = get_animatable<float>(**(parameters.child("fov")));
    m_projection.reset(new projection_matrix(fov));
}

camera_builder::camera_builder(
    transformation const& transformation,
    std::shared_ptr<projection_matrix> projection,
    std::vector<animatable_value<math::plane>> clipping_planes,
    bool flip_culling_mode)
: m_transformation(&transformation)
, m_projection(projection)
, m_clipping_planes(clipping_planes)
, m_flip_culling_mode(flip_culling_mode)
{
}

rendering::camera camera_builder::camera() const
{
    return rendering::camera(
        create_camera_variables(*m_transformation, *m_projection, m_clipping_planes),
        m_projection,
        m_clipping_planes,
        m_flip_culling_mode);
}

//std::shared_ptr<rendering::camera> camera_builder::shared_camera() const
//{
//    return std::make_shared<rendering::camera>(
//        create_camera_variables(*m_transformation, *m_projection),
//        m_projection);
//}

std::unique_ptr<rendering::camera> camera_builder::unique_camera() const
{
    return std::make_unique<rendering::camera>(
        create_camera_variables(*m_transformation, *m_projection, m_clipping_planes),
        m_projection,
        m_clipping_planes,
        m_flip_culling_mode);
}

}}} // namespace pt::d3d9::rendering
