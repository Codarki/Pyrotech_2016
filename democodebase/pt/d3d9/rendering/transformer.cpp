#include "transformer.h"
#include "transformer_builder.h"
#include "transformation.h"
#include "pt/any_error.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 { namespace rendering {

transformer::transformer(
    std::shared_ptr<rendering::transformation> transformation_,
    std::shared_ptr<rendering::transformation> default_pose)
: m_transformation(transformation_)
, m_default_pose(default_pose)
{
}

void transformer::set_parent(transformer const& transformer)
{
    if (m_transformation)
        m_transformation->set_parent(transformer.shared_transformation());

    if (m_default_pose)
        m_default_pose->set_parent(transformer.shared_default_pose());
}

void transformer::clear_parent()
{
    m_transformation->set_parent(nullptr);
}

bool transformer::has_transformation() const
{
    return m_transformation != 0;
}

rendering::transformation const& transformer::transformation() const
{
    assert(m_transformation);
    return *m_transformation;
}

std::shared_ptr<rendering::transformation>
    transformer::shared_transformation() const
{
    return m_transformation;
}

//std::shared_ptr<rendering::transformation> transformer::shared_transformation()
//{
//    return m_transformation;
//}

bool transformer::has_default_pose() const
{
    return m_default_pose != 0;
}

rendering::transformation const& transformer::default_pose() const
{
    assert(m_transformation);
    return *m_transformation;
}

std::shared_ptr<rendering::transformation>
    transformer::shared_default_pose() const
{
    return m_default_pose;
}

//std::shared_ptr<rendering::transformation> transformer::shared_default_pose()
//{
//    return m_default_pose;
//}

// Free functions

matrix4x4f local_to_world_at(transformer const& transformer, float time)
{
    return transformation_from(transformer).local_to_world_at(time);
}

matrix4x4f world_to_local_at(transformer const& transformer, float time)
{
    return invert(local_to_world_at(transformer, time));
}

transformation const& transformation_from(transformer const& transformer)
{
    if (transformer.has_transformation())
        return transformer.transformation();
    else if (transformer.has_default_pose())
        return transformer.default_pose();

    throw any_error("null transformer");
}

std::shared_ptr<transformation> shared_transformation_from(transformer const& transformer)
{
    if (transformer.has_transformation())
        return transformer.shared_transformation();
    else if (transformer.has_default_pose())
        return transformer.shared_default_pose();

    throw any_error("null transformer");
}

}}} // namespace pt::d3d9::rendering
