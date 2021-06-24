#include "write_material.h"
#include "../file_3ds/chunk_ids.h"
#include "../file_3ds/material.h"
#include "../file_3ds/material_map.h"
#include "pt/any_error.h"
#include "pt/file_output_stream.h"
#include "pt/vector_utility.h"
#include "pt/path.h"
//#include "pt/boost/filesystem/operations.h"
//#include "pt/boost/filesystem/convenience.h"
#include "p3d_format/chunk_writer.h"
#include "p3d_format/id.h"

namespace pt { namespace conversion { namespace p3d {

namespace {

    p3d_format::chunk_id p3d_id_for_3ds_id(uint32 id)
    {
        switch(id)
        {
            case file_3ds::CHUNK_TEXTURE_MAP_1:
                return p3d_format::id_texture_map_1;
            case file_3ds::CHUNK_TEXTURE_MAP_2:
                return p3d_format::id_texture_map_2;
            case file_3ds::CHUNK_OPACITY_MAP:
                return p3d_format::id_opacity_map;
            case file_3ds::CHUNK_BUMP_MAP:
                return p3d_format::id_bump_map;
            case file_3ds::CHUNK_SHININESS_MAP:
                return p3d_format::id_shininess_map;
            case file_3ds::CHUNK_SPECULAR_MAP:
                return p3d_format::id_specular_map;
            case file_3ds::CHUNK_SELF_ILLUM_MAP:
                return p3d_format::id_self_illumination_map;
            case file_3ds::CHUNK_REFLECTION_MAP:
                return p3d_format::id_reflection_map;
            case file_3ds::CHUNK_MASK_FOR_TEXTURE_MAP_1:
                return p3d_format::id_mask_for_texture_map_1;
            case file_3ds::CHUNK_MASK_FOR_TEXTURE_MAP_2: 
                return p3d_format::id_mask_for_texture_map_2;
            case file_3ds::CHUNK_MASK_FOR_OPACITY_MAP:
                return p3d_format::id_mask_for_opacity_map;
            case file_3ds::CHUNK_MASK_FOR_BUMP_MAP:
                return p3d_format::id_mask_for_bump_map;
            case file_3ds::CHUNK_MASK_FOR_SHININESS_MAP:
                return p3d_format::id_mask_for_shininess_map;
            case file_3ds::CHUNK_MASK_FOR_SPECULAR_MAP:
                return p3d_format::id_mask_for_specular_map;
            case file_3ds::CHUNK_MASK_FOR_SELF_ILLUM_MAP:
                return p3d_format::id_mask_for_self_illumination_map;
            case file_3ds::CHUNK_MASK_FOR_REFLECTION_MAP:
                return p3d_format::id_mask_for_reflection_map;
            default:
                throw any_error("unknown texture type");
        };
    }
}

void write_scene_material(boost::shared_ptr<file_3ds::material> const& material,
    file_output_stream& s, path const& image_directory)
{
    assert(material);

    p3d_format::chunk_writer chunk(s, p3d_format::id_material);

    chunk.write<std::string>(material->name());
    chunk.write<vector3f>(material->ambient());
    chunk.write<vector3f>(material->diffuse());
    chunk.write<vector3f>(material->specular());
    chunk.write<float>(material->shininess() / 100.0f);
    chunk.write<float>(material->shininess_strength());
    chunk.write<float>(material->transparency());
    chunk.write<float>(material->transparency_falloff());
    chunk.write<float>(material->reflection_blur());
    chunk.write(static_cast<int8>(material->maps().size()));

    for (file_3ds::material::map_map::const_iterator i
        = material->maps().begin(); i != material->maps().end(); ++i)
    {
        p3d_format::chunk_writer texture_chunk(s, p3d_id_for_3ds_id(i->first));

		boost::shared_ptr<file_3ds::material_map> const& map = i->second;
        path map_filename = image_directory / map->filename();
        //map_filename = filesystem::change_extension(map_filename, ".dds");
        //map_filename = filesystem::change_extension(map_filename, ".jpg");

        pt::create_directories(map_filename.parent_path());

        texture_chunk.write(map_filename.string());
        texture_chunk.write<float>(map->blur_percent());
        texture_chunk.write<float>(map->unknown_percent());
        texture_chunk.write<uint16>(map->mapping_parameters());
        texture_chunk.write<vector2f>(map->uv_scale());
        texture_chunk.write<vector2f>(map->uv_offset());
    }
}

}}} // namespace pt::conversion::p3d
