#include "item_geometry.h"
#include "geometry.h"
#include "transformation_variables.h"

namespace pt { namespace d3d9 { namespace rendering {

item_geometry::item_geometry(
    std::shared_ptr<rendering::geometry> geometry,
    std::shared_ptr<rendering::transformation_variables> variables)
: geometry_(geometry)
, variables_(variables)
{
    assert(geometry_);
    assert(variables_);
}

rendering::geometry& item_geometry::geometry()
{
    return *geometry_;
}

rendering::geometry const& item_geometry::geometry() const
{
    return *geometry_;
}

transformation_variables& item_geometry::variables()
{
    return *variables_;
}

transformation_variables const& item_geometry::variables() const
{
    return *variables_;
}

std::shared_ptr<transformation_variables> item_geometry::shared_variables()
{
    return variables_;
}

// Free functions

std::unique_ptr<item_geometry> build_unique_item_geometry(
    std::shared_ptr<rendering::geometry> geometry,
    transformation const& transformation)
{
    auto variables = std::make_shared<rendering::transformation_variables>(transformation);
    return std::make_unique<item_geometry>(geometry, move(variables));
}

}}} // namespace pt::d3d9::rendering
