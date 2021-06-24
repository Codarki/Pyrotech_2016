#if 0
#ifndef PT_CONVERSION_FILE_3DS_DEFS_H
#define PT_CONVERSION_FILE_3DS_DEFS_H

#include "chunk_ids.h"
#include "pt/bounded_input_stream.h"
#include "pt/basic_types.h"
#include "pt/vector_utility.h"
#include "pt/stream_content.h"

namespace pt { namespace conversion { namespace file_3ds {

/// Info for .3ds chuck.
struct chunk_info
{
	uint16 id;
	uint32 len;

    size_t start_offset;
    size_t end_offset;

    bool is(CHUNK_TYPE type)
    {
        return id == type;
    }
};

/// Reads chunk info from \a stream.
inline void read_chunk_info(chunk_info& info, bounded_input_stream& stream)
{
    info.start_offset = stream.position();
    info.id = pt::read<uint16>(stream);
    info.len = pt::read<uint32>(stream);
    info.end_offset = info.start_offset + info.len;

    assert(info.end_offset <= stream.contents().size());
}

/// Skips a chunk info from \a stream.
inline void skip_chunk(chunk_info& info, bounded_input_stream& stream)
{
    stream.seek(info.end_offset);
}

/// \return C string from \a stream.
inline std::string read_c_str(bounded_input_stream& stream)
{
    std::string name;
    unsigned char c;
    while((c = pt::read<unsigned char>(stream)) != 0)
        name += c;
    return name;
}

/// \return Color from \a stream.
inline vector3f read_color_chunk(chunk_info const& parent_info,
    bounded_input_stream& stream)
{
    vector3f value;
    while(stream.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, stream);

        if(info.is(CHUNK_RGBF))
        {
            value = pt::read<vector3f>(stream);
        }
        else if(info.is(CHUNK_RGBB))
        {
            for(size_t i = 0; i < 3; ++i)
            {
                value[i] = pt::read<uint8>(stream) / 255.0f;
            }
        }
        else if(info.is(CHUNK_RGBB_GAMMA_CORRECTED))
        {
            for(size_t i = 0; i < 3; ++i)
            {
                value[i] = pt::read<uint8>(stream) / 255.0f;
            }
        }
        else if(info.is(CHUNK_RGBF_GAMMA_CORRECTED))
        {
            value = pt::read<vector3f>(stream);
        }
    }
    return value;
}

/// \return Percent value \a stream.
inline float read_percent_chunk(chunk_info const&, bounded_input_stream& s)
{
    chunk_info info;
    read_chunk_info(info, s);

    float value = 0;
    if(info.is(CHUNK_PERCENTI))
    {
        value = static_cast<float>(pt::read<uint16>(s));
    }
    else if(info.is(CHUNK_PERCENTF))
    {
        value = pt::read<float>(s);
    }
    skip_chunk(info, s);
    return value;
}

}}} // namespace pt::conversion::file_3ds

#endif
#endif
