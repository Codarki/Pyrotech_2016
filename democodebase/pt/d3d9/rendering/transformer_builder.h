#pragma once
#ifndef PT_D3D9_RENDERING_TRANSFORMER_BUILDER_H
#define PT_D3D9_RENDERING_TRANSFORMER_BUILDER_H

#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class transformation;
class transformer;

/// Utility class which helps you build immutable transformers.
class transformer_builder
{
public:
    transformer_builder();
    transformer_builder(std::shared_ptr<transformation> transformation);
	transformer_builder(
        std::shared_ptr<transformation> transformation_,
        std::shared_ptr<transformation> default_pose);

    /// Every call returns new transformer with shared transformations.
	//std::shared_ptr<rendering::transformer> shared_transformer() const;
    /// Every call returns new transformer.
    std::unique_ptr<rendering::transformer> unique_transformer() const;

private: // data members
    std::shared_ptr<transformation> m_transformation;
    std::shared_ptr<transformation> m_default_pose;
};

}}} // namespace pt::d3d9::rendering

#endif
