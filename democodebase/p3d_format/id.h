#ifndef PT_P3D_FORMAT_ID_H
#define PT_P3D_FORMAT_ID_H

#include "pt/basic_types.h"
#include "pt/std/string.h"

namespace p3d_format {

#define PT_CREATE_ID(a,b,c,d) ((a << 24) + (b << 16) + (c << 8) + d)

enum chunk_id
{
    id_scene        = PT_CREATE_ID('P','3','D','S'),
    id_mesh         = PT_CREATE_ID('M','E','S','H'),
    id_light        = PT_CREATE_ID('L','G','H','T'),
    id_camera       = PT_CREATE_ID('C','A','M',' '),
    id_material     = PT_CREATE_ID('M','T','R','L'),
    id_animation    = PT_CREATE_ID('A','N','I','M'),

    id_mesh_model   = PT_CREATE_ID('P','3','D','M'),

    // texture maps
    id_texture_map_1                    = PT_CREATE_ID('T','1',' ',' '),
    id_texture_map_2                    = PT_CREATE_ID('T','2',' ',' '),
    id_opacity_map                      = PT_CREATE_ID('T','O',' ',' '),
    id_bump_map                         = PT_CREATE_ID('T','B',' ',' '),
    id_shininess_map                    = PT_CREATE_ID('T','S','H',' '),
    id_specular_map                     = PT_CREATE_ID('T','S','P',' '),
    id_self_illumination_map            = PT_CREATE_ID('T','S','I',' '),
    id_reflection_map                   = PT_CREATE_ID('T','R',' ',' '),
    id_mask_for_texture_map_1           = PT_CREATE_ID('T','M','1',' '),
    id_mask_for_texture_map_2           = PT_CREATE_ID('T','M','2',' '),
    id_mask_for_opacity_map             = PT_CREATE_ID('T','M','O',' '),
    id_mask_for_bump_map                = PT_CREATE_ID('T','M','B',' '),
    id_mask_for_shininess_map           = PT_CREATE_ID('T','M','S','H'),
    id_mask_for_specular_map            = PT_CREATE_ID('T','M','S','P'),
    id_mask_for_self_illumination_map   = PT_CREATE_ID('T','M','S','I'),
    id_mask_for_reflection_map          = PT_CREATE_ID('T','M','R',' '),
};

#undef PT_CREATE_ID

std::string id_name(pt::uint32 id);
pt::uint32 id_for_name(std::string const& name);

} // namespace p3d_format

#endif
