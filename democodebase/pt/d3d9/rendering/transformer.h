#pragma once
#ifndef PT_D3D9_RENDERING_TRANSFORMER_H
#define PT_D3D9_RENDERING_TRANSFORMER_H

#include "pt/matrix_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class transformation;

/// Item transformer. Handles item hierarchy transformations.
class transformer
{
public:
    transformer(std::shared_ptr<rendering::transformation> transformation_,
        std::shared_ptr<rendering::transformation> default_pose);

    void set_parent(transformer const& transformer);
    void clear_parent();

    bool has_transformation() const;
    rendering::transformation const& transformation() const;
    //std::shared_ptr<rendering::transformation> shared_transformation();

    bool has_default_pose() const;
    rendering::transformation const& default_pose() const;
    //std::shared_ptr<rendering::transformation> shared_default_pose();

public:
    std::shared_ptr<rendering::transformation> shared_transformation() const;
    std::shared_ptr<rendering::transformation> shared_default_pose() const;

private: // data members
    std::shared_ptr<rendering::transformation> m_transformation;
    std::shared_ptr<rendering::transformation> m_default_pose;
};

matrix4x4f local_to_world_at(transformer const& transformer, float time);
matrix4x4f world_to_local_at(transformer const& transformer, float time);

transformation const& transformation_from(transformer const& transformer);
std::shared_ptr<transformation> shared_transformation_from(transformer const& transformer);

}}} // namespace pt::d3d9::rendering

#endif
