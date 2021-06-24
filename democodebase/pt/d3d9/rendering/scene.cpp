#include "scene.h"
#include "item_name.h"
#include "transformer.h"
#include "pt/any_error.h"
#include "pt/format.h"
#include "pt/parameter_collection.h"
#include "pt/std/cassert.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class scene::impl
{
public:
    impl()
    {
    }
    
    void add_camera(std::shared_ptr<rendering::item>&& item)
    {
        assert(item != 0);
        m_items_with_camera.push_back(item.get());
        m_managed_items_with_camera.push_back(std::move(item));
    }

    void add_light(std::shared_ptr<rendering::item>&& item)
    {
        assert(item != 0);
        m_items_with_light.push_back(item.get());
        m_managed_items_with_light.push_back(std::move(item));
    }

    size_t camera_count() const
    {
        return m_items_with_camera.size();
    }

    size_t light_count() const
    {
        return m_items_with_light.size();
    }

    rendering::item** cameras()
    {
        return m_items_with_camera.data();
    }

    rendering::item** lights()
    {
        return m_items_with_light.data();
    }

    rendering::item const* const* lights() const
    {
        return m_items_with_light.data();
    }

private:
    std::vector<std::shared_ptr<rendering::item> > m_managed_items_with_camera;
    std::vector<std::shared_ptr<rendering::item> > m_managed_items_with_light;
    std::vector<rendering::item*> m_items_with_camera;
    std::vector<rendering::item*> m_items_with_light;
};

scene::scene(item_name const& name,
    std::unique_ptr<rendering::transformer>&& transformer)
: item(name, move(transformer))
, m_pimpl(new impl)
{
}

scene::~scene()
{
    delete m_pimpl;
}

void scene::add_camera_item(std::shared_ptr<rendering::item> item)
{
    m_pimpl->add_camera(std::move(item));
}

void scene::add_light_item(std::shared_ptr<rendering::item> light)
{
    m_pimpl->add_light(std::move(light));
}

size_t scene::camera_count() const
{
    return m_pimpl->camera_count();
}

size_t scene::light_count() const
{
    return m_pimpl->light_count();
}

item** scene::cameras()
{
    return m_pimpl->cameras();
}

item** scene::lights()
{
    return m_pimpl->lights();
}

item const* const* scene::lights() const
{
    return m_pimpl->lights();
}

// free functions

item& get_item_with_camera(item_name const& name, scene& scene)
{
    item* item = find_item_with_camera(name, scene);
    if (item != 0)
        return *item;

    throw any_error(pt::format("Item \"%1%\" with a camera not found in scene \"%2%\"")
        % name.text() % scene.name().text());
}

item& get_item_with_light(item_name const& name, scene& scene)
{
    item* item = find_item_with_light(name, scene);
    if (item != 0)
        return *item;

    throw any_error(pt::format("Item \"%1%\" with a light not found in scene \"%2%\"")
        % name.text() % scene.name().text());
}

item* find_item_with_camera(item_name const& name, scene& scene)
{
    size_t const camera_count = scene.camera_count();

    for (size_t i = 0; i < camera_count; ++i)
    {
        item* item = scene.cameras()[i];
        if (item->name() == name)
            return item;
    }

    return 0;
}

item* find_item_with_light(item_name const& name, scene& scene)
{
    size_t const light_count = scene.light_count();

    for (size_t i = 0; i < light_count; ++i)
    {
        item* item = scene.lights()[i];
        if (item->name() == name)
            return item;
    }

    return 0;
}

}}} // namespace pt::d3d9::rendering
