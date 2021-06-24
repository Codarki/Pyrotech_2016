#pragma once
#ifndef PT_D3D9_RENDERING_ITEM_GEOMETRY_H
#define PT_D3D9_RENDERING_ITEM_GEOMETRY_H

#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class geometry;
class transformation;
class transformation_variables;
class variable_owner;

class item_geometry
{
public:
    item_geometry(
        std::shared_ptr<rendering::geometry> geometry,
        std::shared_ptr<rendering::transformation_variables> variables);

    rendering::geometry& geometry();
    rendering::geometry const& geometry() const;

    transformation_variables& variables();
    transformation_variables const& variables() const;
    std::shared_ptr<transformation_variables> shared_variables();

private:
    std::shared_ptr<rendering::geometry> geometry_;
    std::shared_ptr<rendering::transformation_variables> variables_;
};

std::unique_ptr<item_geometry> build_unique_item_geometry(
    std::shared_ptr<rendering::geometry> geometry,
    transformation const& transformation);

}}} // namespace pt::d3d9::rendering

#endif
