#ifndef PT_CONVERSION_FILE_3DS_CAMERA_H
#define PT_CONVERSION_FILE_3DS_CAMERA_H

#include "pt/boost/smart_ptr.h"
#include "pt/vector.h"
#include "object.h"

namespace pt {
class bounded_input_stream;
    
namespace conversion { namespace file_3ds {

struct chunk_info;

/// Internalizes 3ds camera.
class camera : public object
{
public:
    camera(std::string const& name);

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    vector3f const& position() const;
    vector3f const& target() const;
    float bank() const;
    float lens() const;

public: // object virtual function implementations
    bool is_mesh() const {return false;}
    bool is_light() const {return false;}
    bool is_camera() const {return true;}

private: // helper functions
private: // data members
    vector3f m_position;
    vector3f m_target;
    float m_bank;
    float m_lens;
};

/// \return Camera read from \a stream.
boost::shared_ptr<camera> read_camera(chunk_info const& parent_info,
    std::string const& name, bounded_input_stream& stream);

}}} // namespace pt::conversion::file_3ds

#endif
