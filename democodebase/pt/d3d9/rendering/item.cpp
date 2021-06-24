#include "item.h"
#include "item_collection.h"
#include "item_geometry.h"
#include "item_name.h"
#include "light.h"
#include "transformer.h"
#include "view.h"
#include "pt/parameter_collection.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

class item::impl
{
public:
    impl(item_name const& name,
        std::unique_ptr<rendering::transformer>&& transformer)
    : m_name(name)
    , m_transformer(move(transformer))
    {
        assert(m_transformer);
    }

    item_name const& name() const
    {
        return m_name;
    }

    item_collection const& children() const
    {
        return m_children;
    }

    item_collection& children()
    {
        return m_children;
    }

	void push_back(std::shared_ptr<item> const& child)
    {
        m_children.push_back(child);

        child->transformer().set_parent(*m_transformer);
    }

	void erase(std::shared_ptr<item> const& child)
    {
        m_children.erase(child);
        child->transformer().clear_parent();
    }

    void set_geometry(std::unique_ptr<item_geometry> geometry)
    {
        m_geometry = move(geometry);
    }

    item_geometry const* geometry_ptr() const
    {
        return m_geometry.get();
    }

    item_geometry* geometry_ptr()
    {
        return m_geometry.get();
    }

	rendering::transformer const& transformer() const
    {
        return *m_transformer;
    }

	rendering::transformer& transformer()
    {
        return *m_transformer;
    }

public: // public data members
    item_name m_name;
    item_collection m_children;
    std::unique_ptr<rendering::transformer> m_transformer;

    std::unique_ptr<view> m_camera;
    std::unique_ptr<item_geometry> m_geometry;
    std::unique_ptr<rendering::light> m_light;
};

item::item(item_name const& name,
    std::unique_ptr<rendering::transformer>&& transformer)
: m_impl(new impl(name, move(transformer)))
{
}

item::~item()
{
}

item_name const& item::name() const
{
    return m_impl->name();
}

item_collection const& item::children() const
{
    return m_impl->children();
}

item_collection& item::children()
{
    return m_impl->children();
}

void item::push_back(std::shared_ptr<item> const& child)
{
    m_impl->push_back(child);
}

void item::erase(std::shared_ptr<item> const& child)
{
    m_impl->erase(child);
}

void item::set_camera(std::unique_ptr<view>&& camera)
{
    m_impl->m_camera = move(camera);
}

view const& item::camera() const
{
    return *m_impl->m_camera;
}

view& item::camera()
{
    return *m_impl->m_camera;
}

void item::set_geometry(std::unique_ptr<item_geometry>&& geometry)
{
    m_impl->set_geometry(move(geometry));
}

item_geometry const& item::geometry() const
{
    return *m_impl->geometry_ptr();
}

item_geometry& item::geometry()
{
    return *m_impl->geometry_ptr();
}

void item::set_light(std::unique_ptr<rendering::light>&& light)
{
    m_impl->m_light = move(light);
}

rendering::light const& item::light() const
{
    return *m_impl->m_light;
}

rendering::light& item::light()
{
    return *m_impl->m_light;
}

bool item::has_camera()
{
    return m_impl->m_camera != nullptr;
}

bool item::has_geometry()
{
    return m_impl->geometry_ptr() != nullptr;
}

bool item::has_light()
{
    return m_impl->m_light != nullptr;
}

rendering::transformer const& item::transformer() const
{
    return m_impl->transformer();
}

rendering::transformer& item::transformer()
{
    return m_impl->transformer();
}

// Free functions

namespace {
    /// Returns \c true if \a parent is the parent of the \a child.
    bool is_parent(item& parent, item& child)
    {
        for (item_collection::const_iterator i = parent.children().begin();
            i != parent.children().end(); ++i)
        {
			auto const& item = *i;

            if (item.get() == &child)
                return true;
        }

        return false;
    }

	std::shared_ptr<item> find_parent_for_item_recursively(
		std::shared_ptr<item> root, item& child)
    {
        if (is_parent(*root, child))
            return root;

        for (item_collection::const_iterator i = root->children().begin();
            i != root->children().end(); ++i)
        {
			if (std::shared_ptr<item> j
                = find_parent_for_item_recursively(*i, child))
            {
                return j;
            }
        }

		return nullptr;
    }
} // anonymous namespace

std::shared_ptr<item> parent_for_item(std::shared_ptr<item> root, item* child)
{
    assert(child);
	if (std::shared_ptr<item> item
        = find_parent_for_item_recursively(root, *child))
    {
        return item;
    }

    throw any_error("item is not a child in given item hierarchy");
}

#pragma warning(push)
#pragma warning(disable: 4702) // unreachable code

item* find_item(item_name const& name, item& root)
{
    if (root.name() == name)
    {
        return &root;
    }

    for (auto const& child : root.children())
    {
        auto found_item = find_item(name, *child);
        if (found_item)
            return found_item;
    }

	return nullptr;
}

item* get_item(item_name const& name, item& root)
{
    if (root.name() == name)
    {
        return &root;
    }

    for (auto const& child : root.children())
    {
        auto found_item = find_item(name, *child);
        if (found_item)
            return found_item;
    }

    throw any_error(format("%1% not found") % name.text());
    return nullptr;
}
#pragma warning(pop)

//void update_recursively(item& item, float time,
//    matrix4x4f parent_local_to_world)
//{
//    matrix4x4f local_to_world;
//
//    if (item.transformation())
//    {
//        local_to_world = item.transformation()->local_to_parent_at(time)
//            * parent_local_to_world;
//    }
//    else if (item.default_pose())
//    {
//        local_to_world = item.default_pose()->local_to_parent_at(time)
//            * parent_local_to_world;
//    }
//
//    item.transformer()->set_local_to_world(local_to_world);
//
//    for (item::iterator i = item.begin(); i != item.end(); ++i)
//    {
//        rendering::item& child = *(*i);
//
//        update_recursively(child, time, local_to_world);
//    }
//}

}}} // namespace pt::d3d9::rendering
