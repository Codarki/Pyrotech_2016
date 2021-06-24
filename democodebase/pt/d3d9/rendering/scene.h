#ifndef PT_D3D9_RENDERING_SCENE_H
#define PT_D3D9_RENDERING_SCENE_H

#include "item.h"

namespace pt { namespace d3d9 { namespace rendering {

class scene : public item
{
public:
    scene(item_name const& name,
		std::unique_ptr<rendering::transformer>&& transformer);
    ~scene();
    
    void add_camera_item(std::shared_ptr<rendering::item> item);
    void add_light_item(std::shared_ptr<rendering::item> light);

    size_t camera_count() const;
    size_t light_count() const;

    /// Returns array of pointers to items with a camera.
    item** cameras();
    /// Returns array of pointers to items with a light.
    rendering::item** lights();
    /// Returns array of pointers to items with a light.
    rendering::item const* const* lights() const;

private:
    scene(scene const&);
    void operator=(scene const&);

    class impl;
    impl* m_pimpl;
};

item& get_item_with_camera(item_name const& name, scene& scene);
item& get_item_with_light(item_name const& name, scene& scene);

item* find_item_with_camera(item_name const& name, scene& scene);
item* find_item_with_light(item_name const& name, scene& scene);

}}} // namespace pt::d3d9::rendering

#endif
