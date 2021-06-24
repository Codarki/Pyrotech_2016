#ifndef PT_CONVERSION_FILE_3DS_DATA_H
#define PT_CONVERSION_FILE_3DS_DATA_H

#include "pt/std/vector.h"
#include "pt/boost/smart_ptr.h"

namespace pt {
    class bounded_input_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {

struct chunk_info;

class material;
class object;
class keyframer;

/// Class containing the data imported from .3ds file.
class data
{
public: // type definitions
	typedef std::vector<boost::shared_ptr<object> > object_container;
	typedef std::vector<boost::shared_ptr<material> > material_container;

public:
    data();

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    object_container const& objects() const;
    material_container const& materials() const;
	boost::shared_ptr<file_3ds::keyframer const> keyframer() const;

    size_t frame_count() const;

private: // data members
    object_container m_objects;
    material_container m_materials;
	boost::shared_ptr<file_3ds::keyframer> m_keyframer;
};

}}} // namespace pt::conversion::file_3ds

#endif
