#include "transformer_variables.h"
#include "reference_setter.h"
#include "pt/d3d9/constant.h"

namespace pt { namespace d3d9 { namespace rendering {

transformer_variables::transformer_variables()
: m_local_to_world(matrix4x4f())
, m_world_to_local(matrix4x4f())
, m_local_to_world_name("object_to_world")
, m_world_to_local_name("world_to_object")
{
}

transformer_variables::transformer_variables(
    matrix4x4f const& local_to_world, matrix4x4f const& world_to_local)
: m_local_to_world(local_to_world)
, m_world_to_local(world_to_local)
, m_local_to_world_name("object_to_world")
, m_world_to_local_name("world_to_object")
{
}

transformer_variables::~transformer_variables()
{
}

void transformer_variables::set_local_to_world(matrix4x4f const& matrix)
{
    m_local_to_world = matrix;
}

void transformer_variables::set_world_to_local(matrix4x4f const& matrix)
{
    m_world_to_local = matrix;
}

void transformer_variables::set_local_to_world_name(std::string const& name)
{
    m_local_to_world_name = name;
}

void transformer_variables::set_world_to_local_name(std::string const& name)
{
    m_world_to_local_name = name;
}

std::unique_ptr<constant_setter> transformer_variables::on_create_constant(
    constant const& constant) const
{
    if (constant.name() == m_local_to_world_name)
    {
        return std::unique_ptr<constant_setter>(
            new reference_setter<matrix4x4f>(m_local_to_world.mutable_reference()));
    }

    if (constant.name() == m_world_to_local_name)
    {
        return std::unique_ptr<constant_setter>(
            new reference_setter<matrix4x4f>(m_world_to_local.mutable_reference()));
    }

    return nullptr;
}

}}} // namespace pt::d3d9::rendering
