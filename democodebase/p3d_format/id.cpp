#include "id.h"
#include "pt/any_error.h"
#include "pt/format.h"

namespace p3d_format {

using namespace pt;

std::string id_name(uint32 id)
{
    switch(id)
    {
    case id_scene:      return "id_scene";
    case id_mesh:       return "id_mesh";
    case id_light:      return "id_light";
    case id_camera:     return "id_camera";
    case id_material:   return "id_material";
    case id_animation:  return "id_animation";
    case id_mesh_model: return "id_mesh_model";
    case id_texture_map_1:  return "id_texture_map_1";
    case id_texture_map_2:  return "id_texture_map_2";
    case id_opacity_map:    return "id_opacity_map";
    case id_bump_map:       return "id_bump_map";
    case id_shininess_map:  return "id_shininess_map";
    case id_specular_map:   return "id_specular_map";
    case id_self_illumination_map:  return "id_self_illumination_map";
    case id_reflection_map:         return "id_reflection_map";
    case id_mask_for_texture_map_1: return "id_mask_for_texture_map_1";
    case id_mask_for_texture_map_2: return "id_mask_for_texture_map_2";
    case id_mask_for_opacity_map:   return "id_mask_for_opacity_map";
    case id_mask_for_bump_map:      return "id_mask_for_bump_map";
    case id_mask_for_shininess_map: return "id_mask_for_shininess_map";
    case id_mask_for_specular_map:  return "id_mask_for_specular_map";
    case id_mask_for_self_illumination_map: return "id_mask_for_self_illumination_map";
    case id_mask_for_reflection_map:    return "id_mask_for_reflection_map";
    }
    throw any_error(format("unknown chunk id %1%") % id);
}

uint32 id_for_name(std::string const& name)
{
    if (name == "id_scene")
        return id_scene;
    else if (name == "id_mesh")
        return id_mesh;
    else if (name == "id_light")
        return id_light;
    else if (name == "id_camera")
        return id_camera;
    else if (name == "id_material")
        return id_material;
    else if (name == "id_animation")
        return id_animation;
    else if (name == "id_mesh_model")
        return id_mesh_model;
    else if (name == "id_texture_map_1")
        return id_texture_map_1;
    else if (name == "id_texture_map_2")
        return id_texture_map_2;
    else if (name == "id_opacity_map")
        return id_opacity_map;
    else if (name == "id_bump_map")
        return id_bump_map;
    else if (name == "id_shininess_map")
        return id_shininess_map;
    else if (name == "id_specular_map")
        return id_specular_map;
    else if (name == "id_self_illumination_map")
        return id_self_illumination_map;
    else if (name == "id_reflection_map")
        return id_reflection_map;
    else if (name == "id_mask_for_texture_map_1")
        return id_mask_for_texture_map_1;
    else if (name == "id_mask_for_texture_map_2")
        return id_mask_for_texture_map_2;
    else if (name == "id_mask_for_opacity_map")
        return id_mask_for_opacity_map;
    else if (name == "id_mask_for_bump_map")
        return id_mask_for_bump_map;
    else if (name == "id_mask_for_shininess_map")
        return id_mask_for_shininess_map;
    else if (name == "id_mask_for_specular_map")
        return id_mask_for_specular_map;
    else if (name == "id_mask_for_self_illumination_map")
        return id_mask_for_self_illumination_map;
    else if (name == "id_mask_for_reflection_map")
        return id_mask_for_reflection_map;

    throw any_error("no matching chunk id for " + name);
}

} // namespace p3d_format
