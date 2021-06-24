#ifndef PT_D3D9_RENDERING_SCENE_BUILDER_H
#define PT_D3D9_RENDERING_SCENE_BUILDER_H

#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/path_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt {
    class progress_report;
}

namespace pt { namespace d3d9 { namespace rendering {

class composite_curve_description_collection;
class light;
class scene;
class material_cache;

class scene_builder
{
public:
    scene_builder(
        graphics_device& device,
        path const& path,
        material_cache& materials,
        progress_report* progress,
        bool only_serialize,
        composite_curve_description_collection const& composite_curves);

    std::shared_ptr<scene> built_scene() const;

private: // data members
    std::shared_ptr<scene> m_scene;
};

}}} // namespace pt::d3d9::rendering

#endif
