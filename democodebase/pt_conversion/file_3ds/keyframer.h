#ifndef PT_CONVERSION_FILE_3DS_KEYFRAMER_H
#define PT_CONVERSION_FILE_3DS_KEYFRAMER_H

#include "pt/std/string.h"
#include "pt/std/vector.h"
#include "pt/boost/smart_ptr.h"
#include "pt/vector.h"

namespace pt {
    class bounded_input_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {

struct chunk_info;
class key;

/// 3ds keyframer animation track.
class track
{
public: // type definitions
	typedef std::vector<boost::shared_ptr<key> > key_container;

public:
    track(chunk_info const& parent_info,
        bounded_input_stream& s);

    int type() const;
    unsigned short flag() const;
    key_container const& keys() const;

private: // data members
    int m_type;
    unsigned short m_flag;
    unsigned int m_key_count;

    key_container m_keys;
};

/// Collection of keyframer animation tracks.
class keyframer_node
{
public: // type definitions
	typedef std::vector<boost::shared_ptr<track> > track_container;

public:
    keyframer_node(chunk_info const& parent_info,
        bounded_input_stream& s, bool is_target = false);

    std::string const& name() const;
    unsigned short hierarchy_position() const;
    unsigned short hierarchy_father() const;
    unsigned short obj_flag1() const;
    unsigned short obj_flag2() const;
    vector3f const& pivot_point() const;
    track_container const& tracks() const;
    bool is_target() const;

private: // data members
    std::string m_name;
    unsigned short m_hierarchy_position;
    unsigned short m_hierarchy_father;

    unsigned short m_obj_flag1;
    unsigned short m_obj_flag2;

    bool m_is_target;
    vector3f m_pivot_point;
    track_container m_tracks;
};

/// Internalizes 3D Studio keyframing data.
class keyframer
{
public: // type definitions
	typedef boost::shared_ptr<keyframer_node> node_type;
    typedef std::vector<node_type> node_container;

public:
    keyframer();

    void read(chunk_info const& parent_info, bounded_input_stream& s);

    int frame_start() const;
    int frame_end() const;

    node_container const& nodes() const;

private: // data members
    int m_frame_start;
    int m_frame_end;

    node_container m_nodes;
};

boost::shared_ptr<keyframer> read_keyframer(chunk_info const& parent_info,
    bounded_input_stream& s);

}}} // namespace pt::conversion::file_3ds

#endif
