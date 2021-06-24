#pragma once
#ifndef PT_D3D9_RENDERING_CAMERA_BUILDER_H
#define PT_D3D9_RENDERING_CAMERA_BUILDER_H

#include "pt/math/plane.h"
#include "pt/parameter_collection.h"
#include "pt/animatable_value.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class camera;
class projection_matrix;
class transformation;

class camera_builder
{
public:
    camera_builder(
        transformation const& transformation,
        parameter_collection parameters,
        bool flip_culling_mode);
    camera_builder(
        transformation const& transformation,
        std::shared_ptr<projection_matrix> projection,
        std::vector<animatable_value<math::plane>> clipping_planes,
        bool flip_culling_mode);

    /// Every call returns new camera with shared inputs.
    rendering::camera camera() const;
    /// \overload
	//std::shared_ptr<rendering::camera> shared_camera() const;
    std::unique_ptr<rendering::camera> unique_camera() const;

private: // data members
    parameter_collection m_parameters;
    transformation const* m_transformation;
    std::shared_ptr<projection_matrix> m_projection;
    std::vector<animatable_value<math::plane>> m_clipping_planes;
    bool m_flip_culling_mode;
};

}}} // namespace pt::d3d9::rendering

#endif
