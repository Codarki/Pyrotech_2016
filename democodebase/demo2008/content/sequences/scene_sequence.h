#ifndef CONTENT_SCENE_SEQUENCE_H
#define CONTENT_SCENE_SEQUENCE_H

#include "basic_composite_sequence.h"
#include "rendertarget.h"
#include "pt/path.h"
#include "pt/animatable_value.h"
#include "pt/d3d9/rendering/material_cache_fwd.h"
#include "pt/d3d9/rendering/surface.h"
#include "pt/d3d9/rendering/shading_program.h"
#include "pt/d3d9/rendering/simple_variable_owner.h"
#include "pt/d3d9/rendering/program_cache.h"
#include "pt/d3d9/rendering/blur.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/config_node_fwd.h"
#include "pt/parameter_collection_fwd.h"

namespace pt {
    class progress_report;
}

namespace pt { namespace d3d9 { namespace rendering {
    class light;
    class scene;
    class camera;
    class item;
}}}

namespace content {
    class scene_player;
}

namespace content { namespace sequences {

class scene_cache;

class scene_sequence : public composite_sequence
{
public:
    scene_sequence(d3d9::graphics_device& device, config_node const& config,
        parameter_collection parameters,
        d3d9::rendering::material_cache& materials,
        sequences::scene_cache& scene_cache,
        progress_report* progress,
        bool only_serialize);

public: // interface required by composite_sequence
    string_wrapper const& name() const;
    std::shared_ptr<d3d9::texture_2d> draw(rendertarget& target, float time);
    float start_time() const;
    float end_time() const;
    void set_override_view(d3d9::rendering::item* view);
    void link_inputs(composite_sequence_collection const& sequences);
    size_t child_count() const;
    std::shared_ptr<composite_sequence const> child(size_t index) const;

private: // noncopyable
    scene_sequence(scene_sequence const&);
    scene_sequence const& operator=(scene_sequence const&);

private: // data members
    basic_composite_sequence m_basic;

    pt::path m_path;
    int m_fps;
    float m_start_time;
    float m_start_frame;
    float m_end_frame;

    float m_near_clip_plane;
    float m_far_clip_plane;
    bool  m_ssao_enabled;
    bool m_is_half_resolution;

    std::shared_ptr<animatable_value<std::string> const > m_camera_name;

    std::shared_ptr<scene_player> m_player;
    std::shared_ptr<d3d9::rendering::scene> m_scene;
    
    d3d9::rendering::material_cache& m_materials;
    d3d9::rendering::item* m_override_view;

    //for ssao
    std::shared_ptr<d3d9::rendering::surface> m_ssao_surface;
    std::shared_ptr<d3d9::rendering::shading_program> m_ssao_program;
    std::shared_ptr<d3d9::rendering::blur> m_blur;

	//for shadowmapping
	std::shared_ptr<d3d9::rendering::surface> m_shadow_depth_surface;
	std::shared_ptr<d3d9::rendering::shading_program> m_shadow_depth_program;

};

}} // namespace content::sequences

#endif
