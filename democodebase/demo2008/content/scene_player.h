#ifndef CONTENT_SCENE_PLAYER_H
#define CONTENT_SCENE_PLAYER_H

#include "pt/d3d9/device_child.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/vector.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"
#include "sequences/rendertarget.h"
#include "pt/d3d9/rendering/shading_program.h"
#include "pt/d3d9/rendering/draw_calls.h"

namespace pt { namespace d3d9 { namespace rendering {
    class item;
    //class light;
    class scene;
    //class camera;
    class program_cache;
}}}

namespace content {

using namespace pt;

class scene_player : public d3d9::device_reference
{
public:
    scene_player(d3d9::graphics_device& device,
        std::shared_ptr<d3d9::rendering::scene> scene);
    ~scene_player();

    void draw(content::sequences::rendertarget& target);
	//void draw();
    void draw_with_preshader() const;
    void draw_to_shadowmap(d3d9::rendering::item* item_with_light, std::vector<d3d9::rendering::draw_call>& calls);
    void update(float time);

    void set_view(d3d9::rendering::item* view);
    void set_view(d3d9::rendering::item* view, float near_plane, float far_plane);
    void set_view(std::string const& name);
    void set_view(std::string const& name,float near_plane, float far_plane);
    void set_view_at(size_t index);

    d3d9::rendering::item* get_item_with_view();

	void set_shadow_depth_program(std::shared_ptr<d3d9::rendering::shading_program> program);

private: // helper functions
    //void draw_item(d3d9::rendering::item const& item,
    //    d3d9::rendering::light const* light, float time) const;
    void update_item(d3d9::rendering::item& item,
        d3d9::rendering::item* item_with_light);

private: // data members
    vector2i m_resolution;
    std::shared_ptr<d3d9::rendering::scene> m_scene;
    d3d9::rendering::item* m_item_with_view;
    float m_last_update_time;
	std::shared_ptr<d3d9::rendering::shading_program> m_shadow_depth_program;
};

std::shared_ptr<d3d9::rendering::program_cache> global_program_cache(
    d3d9::graphics_device& device);
void clear_global_program_cache();

} // namespace content

#endif
