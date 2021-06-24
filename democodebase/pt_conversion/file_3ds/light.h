#ifndef PT_CONVERSION_FILE_3DS_LIGHT_H
#define PT_CONVERSION_FILE_3DS_LIGHT_H

#include "pt/boost/smart_ptr.h"
#include "pt/vector.h"
#include "object.h"

namespace pt {
class bounded_input_stream;
    
namespace conversion { namespace file_3ds {

struct chunk_info;

class light : public object
{
public:
    light(std::string const& name);

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    vector3f const& position() const;
    vector3f const& color() const;

    bool is_spotlight() const;
    vector3f const& target() const;
    float hotspot() const;
    float falloff() const;

    vector2f const& range() const;
    float multiplier() const;

public: // object virtual function implementations
    bool is_mesh() const {return false;}
    bool is_light() const {return true;}
    bool is_camera() const {return false;}

private: // helper functions
private: // data members
    vector3f m_position;
    vector3f m_color;

    // spotlight specific
    bool m_is_spotlight;
    vector3f m_target;
    float m_hotspot;
    float m_falloff;
    vector2f m_range;
    float m_multiplier;
};

boost::shared_ptr<light> read_light(chunk_info const& parent_info,
    std::string const& name, bounded_input_stream& s);

}}} // namespace pt::conversion::file_3ds

#endif
