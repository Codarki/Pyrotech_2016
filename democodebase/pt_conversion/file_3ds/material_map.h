#ifndef PT_CONVERSION_FILE_3DS_MATERIAL_MAP_H
#define PT_CONVERSION_FILE_3DS_MATERIAL_MAP_H

#include "pt/boost/smart_ptr.h"
#include "pt/std/string.h"
#include "pt/vector.h"

namespace pt {
class bounded_input_stream;
    
namespace conversion { namespace file_3ds {
struct chunk_info;

/// Single 3ds texture map.
class material_map
{
public:
    material_map();

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    std::string const& filename() const;
    float blur_percent() const;
    float unknown_percent() const;
    unsigned short mapping_parameters() const;
    vector2f const& uv_scale() const;
    vector2f const& uv_offset() const;

private: // data members
    std::string m_filename;
    float m_blur_percent;
    float m_unknown_percent; // opacity?
    unsigned short m_mapping_parameters; // unknown.
    vector2f m_uv_scale;
    vector2f m_uv_offset;
};

boost::shared_ptr<material_map> read_material_map(chunk_info const& parent_info,
    bounded_input_stream& s);

}}} // namespace pt::conversion::file_3ds

#endif
