#include "material.h"
#include "common.h"
#include "material_map.h"
#include "pt/bounded_input_stream.h"
#include "pt/log.h"
#include "pt/string_wrapper.h"
#include "pt/boost/format.h"

namespace pt { namespace conversion { namespace file_3ds {

material::material()
{
}

void material::read(chunk_info const& parent_info, bounded_input_stream& s)
{
    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        bool not_handled_1 = false;
        bool not_handled_2 = false;

        switch(info.id)
        {
            case CHUNK_MATNAME:
                m_name = read_c_str(s);
                break;

            case CHUNK_AMBIENT:
                m_ambient = read_color_chunk(info, s);
                break;

            case CHUNK_DIFFUSE:
                m_diffuse = read_color_chunk(info, s);
                break;

            case CHUNK_SPECULAR:
                m_specular = read_color_chunk(info, s);
                break;

            case CHUNK_SHININESS_PERCENT:
                m_shininess = read_percent_chunk(info, s);
                break;

            case CHUNK_SHININESS_STRENGTH_PERCENT:
                m_shininess_strength = read_percent_chunk(info, s);
                break;

            case CHUNK_TRANSPARENCY_PERCENT:
                m_transparency = read_percent_chunk(info, s);
                break;

            case CHUNK_TRANSPARENCY_FALLOFF_PERCENT:
                m_transparency_falloff = read_percent_chunk(info, s);
                break;

            case CHUNK_REFLECTION_BLUR_PERCENT:
                m_reflection_blur = read_percent_chunk(info, s);
                break;

            default:
                not_handled_1 = true;
        };

        switch(info.id)
        {
            case CHUNK_TEXTURE_MAP_1:
            case CHUNK_TEXTURE_MAP_2:
            case CHUNK_OPACITY_MAP:
            case CHUNK_BUMP_MAP:
            case CHUNK_SHININESS_MAP:
            case CHUNK_SPECULAR_MAP:
            case CHUNK_SELF_ILLUM_MAP:
            case CHUNK_REFLECTION_MAP:
            case CHUNK_MASK_FOR_TEXTURE_MAP_1:
            case CHUNK_MASK_FOR_TEXTURE_MAP_2:
            case CHUNK_MASK_FOR_OPACITY_MAP:
            case CHUNK_MASK_FOR_BUMP_MAP:
            case CHUNK_MASK_FOR_SHININESS_MAP:
            case CHUNK_MASK_FOR_SPECULAR_MAP:
            case CHUNK_MASK_FOR_SELF_ILLUM_MAP:
            case CHUNK_MASK_FOR_REFLECTION_MAP:
                m_maps[info.id] = read_material_map(info, s);
                break;

            default:
                not_handled_2 = true;
        };

        if (not_handled_1 && not_handled_2)
        {
            log((boost::format("Unhandled chunk (%1$x)") % info.id).str());
        }

        skip_chunk(info, s);
    }
}

std::string const& material::name() const
{
    return m_name;
}

vector3f const& material::ambient() const
{
    return m_ambient;
}

vector3f const& material::diffuse() const
{
    return m_diffuse;
}

vector3f const& material::specular() const
{
    return m_specular;
}

float material::shininess() const
{
    return m_shininess;
}

float material::shininess_strength() const
{
    return m_shininess_strength;
}

float material::transparency() const
{
    return m_transparency;
}

float material::transparency_falloff() const
{
    return m_transparency_falloff;
}

float material::reflection_blur() const
{
    return m_reflection_blur;
}

material::map_map const& material::maps() const
{
    return m_maps;
}

boost::shared_ptr<material> read_material(chunk_info const& parent_info, bounded_input_stream& s)
{
	boost::shared_ptr<material> mat(new material);
    mat->read(parent_info, s);
    return mat;
}

}}} // namespace pt::conversion::file_3ds
