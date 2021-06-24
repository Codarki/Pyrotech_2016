#include "common.h"
#include "pt/bounded_input_stream.h"
#include "pt/vector_utility.h"

namespace pt { namespace conversion { namespace file_3ds {

/// Reads chunk info from \a stream.
void read_chunk_info(chunk_info& info, bounded_input_stream& stream)
{
    info.start_offset = stream.position();
    info.id = pt::read<uint16>(stream);
    info.len = pt::read<uint32>(stream);
    info.end_offset = info.start_offset + info.len;

    assert(info.end_offset <= stream.size());
}

/// Skips a chunk info from \a stream.
void skip_chunk(chunk_info& info, bounded_input_stream& stream)
{
    stream.seek(info.end_offset);
}

/// \return C string from \a stream.
std::string read_c_str(bounded_input_stream& stream)
{
    std::string name;
    unsigned char c;
    while((c = pt::read<unsigned char>(stream)) != 0)
        name += c;
    return name;
}

/// \return Color from \a stream.
vector3f read_color_chunk(chunk_info const& parent_info,
    bounded_input_stream& stream)
{
    vector3f value;
    while(stream.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, stream);

        if (info.is(CHUNK_RGBF))
        {
            value = pt::read<vector3f>(stream);
        }
        else if (info.is(CHUNK_RGBB))
        {
            for (size_t i = 0; i < 3; ++i)
            {
                value[i] = pt::read<uint8>(stream) / 255.0f;
            }
        }
        else if (info.is(CHUNK_RGBB_GAMMA_CORRECTED))
        {
            for (size_t i = 0; i < 3; ++i)
            {
                value[i] = pt::read<uint8>(stream) / 255.0f;
            }
        }
        else if (info.is(CHUNK_RGBF_GAMMA_CORRECTED))
        {
            value = pt::read<vector3f>(stream);
        }
    }
    return value;
}

/// \return Percent value \a stream.
float read_percent_chunk(chunk_info const&, bounded_input_stream& s)
{
    chunk_info info;
    read_chunk_info(info, s);

    float value = 0;
    if (info.is(CHUNK_PERCENTI))
    {
        value = static_cast<float>(pt::read<uint16>(s));
    }
    else if (info.is(CHUNK_PERCENTF))
    {
        value = pt::read<float>(s);
    }
    skip_chunk(info, s);
    return value;
}

std::string track_type_as_text(int track_type)
{
    switch(track_type)
    {
        case file_3ds::CHUNK_KF_POSITION_TRACK:
            return "CHUNK_KF_POSITION_TRACK";
        case file_3ds::CHUNK_KF_ROTATION_TRACK:
            return "CHUNK_KF_ROTATION_TRACK";
        case file_3ds::CHUNK_KF_SCALE_TRACK:
            return "CHUNK_KF_SCALE_TRACK";
        case file_3ds::CHUNK_KF_FOV_TRACK:
            return "CHUNK_KF_FOV_TRACK";
        case file_3ds::CHUNK_KF_ROLL_TRACK:
            return "CHUNK_KF_ROLL_TRACK";
        case file_3ds::CHUNK_KF_COLOR_TRACK:
            return "CHUNK_KF_COLOR_TRACK";
        case file_3ds::CHUNK_KF_MORPH_TRACK:
            return "CHUNK_KF_MORPH_TRACK";
        case file_3ds::CHUNK_KF_HOTSPOT_TRACK:
            return "CHUNK_KF_HOTSPOT_TRACK";
        case file_3ds::CHUNK_KF_FALLOFF_TRACK:
            return "CHUNK_KF_FALLOFF_TRACK";
        case file_3ds::CHUNK_KF_HIDE_TRACK:
            return "CHUNK_KF_HIDE_TRACK";
    };

    return "unknown track";
}

}}} // namespace pt::conversion::file_3ds
