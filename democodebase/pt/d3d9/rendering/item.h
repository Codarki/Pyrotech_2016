#pragma once
#ifndef PT_D3D9_RENDERING_ITEM_H
#define PT_D3D9_RENDERING_ITEM_H

//#include "pt/parameter_collection_fwd.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class item_collection;
class item_geometry;
class item_name;
class light;
class transformer;
class view;

/// Item with number of child items.
///
/// This class constructs hierarchical graph of items. Transformation
/// hierarchy is handled automatically.
class item
{
public:
    item(item_name const& name,
        std::unique_ptr<rendering::transformer>&& transformer);
    ~item();

    item_name const& name() const;

    item_collection const& children() const;
    item_collection& children();

	void push_back(std::shared_ptr<item> const& child);
	void erase(std::shared_ptr<item> const& child);

    void set_camera(std::unique_ptr<rendering::view>&& camera);
    rendering::view const& camera() const;
    rendering::view& camera();

    void set_geometry(std::unique_ptr<item_geometry>&& geometry);
    item_geometry const& geometry() const;
    item_geometry& geometry();

    void set_light(std::unique_ptr<rendering::light>&& light);
    rendering::light const& light() const;
    rendering::light& light();

    bool has_camera();
    bool has_geometry();
    bool has_light();

	rendering::transformer const& transformer() const;
	rendering::transformer& transformer();

protected:
    item(item const& other) = delete;

private:
    item const& operator=(item const&) = delete;

private: // data members
    class impl;
    std::unique_ptr<impl> m_impl;
};

/// Returns item which is parent for \a child, searching recursively from
/// \a root.
std::shared_ptr<item> parent_for_item(std::shared_ptr<item> root, item* child);

/// Returns item under \a root with name \a name. Returns null if not found.
item* find_item(item_name const& name, item& root);
/// Returns item under \a root with name \a name. Throws if not found.
item* get_item(item_name const& name, item& root);

//void update_recursively(item& item, float time,
//    matrix4x4f parent_local_to_world);

}}} // namespace pt::d3d9::rendering

#endif
