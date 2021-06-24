#ifndef PT_CONVERSION_FILE_3DS_COMMON_H
#define PT_CONVERSION_FILE_3DS_COMMON_H

#include "chunk_ids.h"
#include "pt/basic_types.h"
#include "pt/vector.h"
#include "pt/std/string.h"

namespace pt {
    class bounded_input_stream;
}

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
void read_chunk_info(chunk_info& info, bounded_input_stream& stream);

/// Skips a chunk info from \a stream.
void skip_chunk(chunk_info& info, bounded_input_stream& stream);

/// \return C string from \a stream.
std::string read_c_str(bounded_input_stream& stream);

/// \return Color from \a stream.
vector3f read_color_chunk(chunk_info const& parent_info,
    bounded_input_stream& stream);

/// \return Percent value \a stream.
float read_percent_chunk(chunk_info const&, bounded_input_stream& s);

std::string track_type_as_text(int track_type);

}}} // namespace pt::conversion::file_3ds

#endif
