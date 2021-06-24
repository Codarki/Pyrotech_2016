#include "transformer_builder.h"
#include "transformer.h"
#include "transformation_variables.h"
#include "simple_transformation.h"
#include "pt/std/string.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

// transformer_builder

transformer_builder::transformer_builder()
{
}

transformer_builder::transformer_builder(
    std::shared_ptr<transformation> transformation)
:   m_transformation(transformation)
{
    assert(m_transformation);
}

transformer_builder::transformer_builder(
	std::shared_ptr<transformation> transformation_,
    std::shared_ptr<transformation> default_pose)
:   m_transformation(transformation_)
,   m_default_pose(default_pose)
{
    //assert(m_transformation);
    assert(m_default_pose);
}

//std::shared_ptr<rendering::transformer>
//    transformer_builder::shared_transformer() const
//{
//    std::shared_ptr<transformation> default_pose
//        = m_default_pose
//        ? m_default_pose
//        : std::make_shared<simple_transformation>(matrix4x4f());
//
//    return std::make_shared<transformer>(m_transformation, default_pose);
//}

std::unique_ptr<rendering::transformer>
    transformer_builder::unique_transformer() const
{
    std::shared_ptr<transformation> default_pose
        = m_default_pose
        ? m_default_pose
        : std::make_shared<simple_transformation>(matrix4x4f());

    return std::make_unique<transformer>(m_transformation, move(default_pose));
}

}}} // namespace pt::d3d9::rendering
