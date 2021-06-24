#ifndef CONTENT_SEQUENCES_SCENE_CACHE_H
#define CONTENT_SEQUENCES_SCENE_CACHE_H

#include "simple_composite_sequence.h"
#include "pt/d3d9/rendering/scene.h"
#include "pt/d3d9/rendering/scene_builder.h"
#include "pt/d3d9/device_child.h"
#include "pt/format.h"
#include "pt/log.h"
#include "pt/std/map.h"

namespace content { namespace sequences {

using namespace pt;

class scene_cache
{
public:
	std::shared_ptr<d3d9::rendering::scene> find_or_load(
		std::string const & name,
        d3d9::graphics_device& device,
        d3d9::rendering::material_cache& materials,
		progress_report* progress,
        bool only_serialize,
        d3d9::rendering::composite_curve_description_collection const& composite_curves)
    {
        map::const_iterator i = m_map.find(name);
        if (i != m_map.end())
        {
            log(format("Loaded scene '%1%' from cache") % name);
            return i->second;
        }

        d3d9::rendering::scene_builder builder(device, name, materials,
            progress, only_serialize, composite_curves);

        std::shared_ptr<d3d9::rendering::scene> scene = builder.built_scene();
        m_map[name] = scene;

        return scene;
    }

private:
    typedef std::map<std::string,std::shared_ptr<d3d9::rendering::scene> >  map;
    map m_map;
};

}} // namespace content::sequences

#endif
