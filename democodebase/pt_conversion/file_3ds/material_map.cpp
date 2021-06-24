#include "material_map.h"
#include "common.h"
#include "pt/bounded_input_stream.h"

namespace pt { namespace conversion { namespace file_3ds {

material_map::material_map()
:   m_uv_scale(1, 1)
,   m_uv_offset(0, 0)
{
}

void material_map::read(chunk_info const& parent_info, bounded_input_stream& s)
{
    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_PERCENTI))
        {
            m_unknown_percent = static_cast<float>(pt::read<uint16>(s));
        }
        else if (info.is(CHUNK_MAPPING_FILENAME))
        {
            m_filename = read_c_str(s);
        }
        else if (info.is(CHUNK_MAPPING_PARAMETERS))
        {
            m_mapping_parameters = pt::read<uint16>(s);
        }
        else if (info.is(CHUNK_BLUR_PERCENT))
        {
            m_blur_percent = pt::read<float>(s);
        }
        else if (info.is(CHUNK_V_SCALE))
        {
            m_uv_scale[1] = pt::read<float>(s);
        }
        else if (info.is(CHUNK_U_SCALE))
        {
            m_uv_scale[0] = pt::read<float>(s);
        }
        else if (info.is(CHUNK_U_OFFSET))
        {
            m_uv_offset[0] = pt::read<float>(s);
        }
        else if (info.is(CHUNK_V_OFFSET))
        {
            m_uv_offset[1] = pt::read<float>(s);
        }
        /*else if (info.is(CHUNK_ROTATION_ANGLE))
        {
            // float?
        }
        else if (info.is(CHUNK_RGB_LUMA_OR_ALPHA_TINT_1))
        {
            // float?
        }
        else if (info.is(CHUNK_RGB_LUMA_OR_ALPHA_TINT_2))
        {
            // float?
        }
        else if (info.is(CHUNK_RGB_TINT_R))
        {
            // float?
        }
        else if (info.is(CHUNK_RGB_TINT_G))
        {
            // float?
        }
        else if (info.is(CHUNK_RGB_TINT_B))
        {
            // float?
        }
        */
        else
        {
            int arr = 1; arr;
        }
        skip_chunk(info, s);
    }
}

std::string const& material_map::filename() const
{
    return m_filename;
}

float material_map::blur_percent() const
{
    return m_blur_percent;
}

float material_map::unknown_percent() const
{
    return m_unknown_percent;
}

unsigned short material_map::mapping_parameters() const
{
    return m_mapping_parameters;
}

vector2f const& material_map::uv_scale() const
{
    return m_uv_scale;
}

vector2f const& material_map::uv_offset() const
{
    return m_uv_offset;
}

boost::shared_ptr<material_map> read_material_map(chunk_info const& parent_info,
    bounded_input_stream& s)
{
	boost::shared_ptr<material_map> map(new material_map());
    map->read(parent_info, s);
    return map;
}

}}} // namespace pt::conversion::file_3ds
