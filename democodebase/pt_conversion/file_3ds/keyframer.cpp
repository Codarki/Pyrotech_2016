#include "keyframer.h"
#include "common.h"
#include "key.h"
#include "pt/vector_utility.h"
#include "pt/boost/format.h"
#include "pt/bounded_input_stream.h"
#include <boost/make_shared.hpp>
#include "pt/log.h"
#include "pt/format.h"
#include "pt/math_utils.h"

namespace pt { namespace conversion { namespace file_3ds {

// track

enum acceleration_data_type
{
    tension_data = 0,
    continuity_data,
    bias_data,
    ease_to_data,
    ease_from_data,
};

namespace {

	boost::shared_ptr<key> make_vector3f(int key_number,
        bounded_input_stream& stream, key::acceleration_params acceleration)
    {
        return boost::make_shared<typed_key<vector3f>>(key_number,
            pt::read<vector3f>(stream), acceleration);

        //return shared_ptr<key>(new typed_key<vector3f>(key_number,
        //    pt::read<vector3f>(stream), acceleration));
    }

	boost::shared_ptr<key> make_float(int key_number,
        bounded_input_stream& stream, key::acceleration_params acceleration)
    {
        return boost::make_shared<typed_key<float>>(key_number,
            pt::read<float>(stream), acceleration);
    }

    boost::shared_ptr<key> make_float_to_radians(int key_number,
        bounded_input_stream& stream, key::acceleration_params acceleration)
    {
        return boost::make_shared<typed_key<float>>(key_number,
            to_radians(pt::read<float>(stream)), acceleration);
    }

    key::acceleration_params read_acceleration(bounded_input_stream& s)
    {
        key::acceleration_params acceleration;

        // Bit 0 : Tension follows         [-1.0, 1.0]
        // Bit 1 : Continuity follows      [-1.0, 1.0]
        // Bit 2 : Bias follows            [-1.0, 1.0]
        // Bit 3 : Ease to follows         [ 0.0, 1.0]
        // Bit 4 : Ease from follows       [ 0.0, 1.0]
        short acceleration_data_flags = pt::read<uint16>(s);

        for (size_t j = 0; j < 5; ++j)
        {
            if (!(acceleration_data_flags & (1 << j)))
                continue;

            float value = pt::read<float>(s);

            //assert(value >= 0.0f);

            switch(j)
            {
            case tension_data:
                acceleration.tension = value;
                break;

            case continuity_data:
                acceleration.continuity = value;
                break;

            case bias_data:
                acceleration.bias = value;
                break;

            case ease_to_data:
                acceleration.ease_to = value;
                break;

            case ease_from_data:
                acceleration.ease_from = value;
                break;

            default:
                assert(false);
            }
        }

        return acceleration;
    }

} // anonymous namespace

track::track(chunk_info const& parent_info, bounded_input_stream& s)
: m_type(parent_info.id)
{
    parent_info;
    // Bits 0-1 : 0 = single, 2 = repeat, 3 = loop
    // Bit 3 : lock X
    // Bit 4 : lock Y
    // Bit 5 : lock Z
    // Bit 7 : unlink X
    // Bit 8 : unlink Y
    // Bit 9 : unlink Z
    m_flag = pt::read<uint16>(s);

    // skip 8 bytes
    for (size_t i = 0; i < 8; ++i)
    {
        int j = pt::read<uint8>(s); j;
    }

    m_key_count = pt::read<uint32>(s);

    m_keys.reserve(m_key_count);

    log(format("3DS: %1%: %2% Keys") % track_type_as_text(m_type) % m_key_count);

//    // Specially inverted to prevent loop-switch
//    if (m_type == CHUNK_KF_POSITION_TRACK)
//    {
//        for (size_t i = 0; i < m_key_count; ++i)
//        {
//            int key_number = pt::read<uint32>(s);
//            key::acceleration_params acceleration = read_acceleration(s);
//            m_keys.push_back(make_vector3f(key_number, s, acceleration));
//        }
//    }
//    if (m_type == CHUNK_KF_ROTATION_TRACK)
//    {
//        for (size_t i = 0; i < m_key_count; ++i)
//        {
//            int key_number = pt::read<uint32>(s);
//            key::acceleration_params acceleration = read_acceleration(s);
//
//            float angle = pt::read<float>(s);
//            vector3f axis = pt::read<vector3f>(s);
//            m_keys.push_back(shared_ptr<key>(new typed_key<axis_angle>(
//                key_number, axis_angle(axis, angle), acceleration)));
//        }
//    }
//else
{
    for (size_t i = 0; i < m_key_count; ++i)
    {
        int key_number = pt::read<uint32>(s);

        key::acceleration_params acceleration = read_acceleration(s);

        switch (m_type)
        {
            case CHUNK_KF_POSITION_TRACK:
                m_keys.push_back(make_vector3f(key_number, s, acceleration));
                break;

            case CHUNK_KF_ROTATION_TRACK:
                {
                    float angle = pt::read<float>(s);
                    vector3f axis = pt::read<vector3f>(s);
					m_keys.push_back(boost::shared_ptr<key>(new typed_key<axis_angle>(
                        key_number, axis_angle(axis, angle), acceleration)));
                }
                break;

            case CHUNK_KF_SCALE_TRACK:
                m_keys.push_back(make_vector3f(key_number, s, acceleration));
                break;

            case CHUNK_KF_FOV_TRACK:    
                m_keys.push_back(make_float(key_number, s, acceleration));
                break;

            case CHUNK_KF_ROLL_TRACK:   
                m_keys.push_back(make_float_to_radians(key_number, s, acceleration));
                break;

            case CHUNK_KF_COLOR_TRACK:
                {
                    int what_to_do = 1; what_to_do;
                }
                break;

            case CHUNK_KF_MORPH_TRACK: 
				m_keys.push_back(boost::shared_ptr<key>(new typed_key<std::string>(
                    key_number, read_c_str(s), acceleration)));
                break;

            case CHUNK_KF_HOTSPOT_TRACK: 
                m_keys.push_back(make_float(key_number, s, acceleration));
                break;

            case CHUNK_KF_FALLOFF_TRACK:
                m_keys.push_back(make_float(key_number, s, acceleration));
                break;

            case CHUNK_KF_HIDE_TRACK:
            /*    {
                    int what_to_do = 1; what_to_do;
                }
				*/
				boost::shared_ptr<key> key = make_float(key_number, s, acceleration);
				log(format(" 3DS: HIDE Track Key #%1%") % key->key_number());
				log(format("  Tension: %1%, Continuity: %2%, Bias: %3%, Ease To: %4%, Ease From: %5%") % key->acceleration().tension 
					% key->acceleration().continuity % key->acceleration().bias % key->acceleration().ease_to % key->acceleration().ease_from
					);
                m_keys.push_back(key);
                break;

        };
     }
} // else
}

int track::type() const
{
    return m_type;
}

unsigned short track::flag() const
{
    return m_flag;
}

track::key_container const& track::keys() const
{
    return m_keys;
}

// keyframer_node

keyframer_node::keyframer_node(chunk_info const& parent_info,
    bounded_input_stream& s, bool is_target)
:   m_is_target(is_target)
{
    if (is_target)
        log("Reading keyframer target node");
    else
        log("Reading keyframer node");
    logging::scoped_indent indent;

    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_KF_HIERARCHY_POSITION))
        {
            m_hierarchy_position = pt::read<uint16>(s);
            log(format("Hierarchy position '%1%'") % m_hierarchy_position);
        }
        else if (info.is(CHUNK_KF_OBJECT_NAME_PARAMETERS_AND_HIERARCHY_FATHER))
        {
            m_name = read_c_str(s);
            // Bit 11 : Hidden
            m_obj_flag1 = pt::read<uint16>(s); 

            // Bit 0 : Show path
            // Bit 1 : Animate smoothing
            // Bit 4 : Object motion blur
            // Bit 6 : Morph materials
            m_obj_flag2 = pt::read<uint16>(s);

            // -1 for none (0xffff)
            m_hierarchy_father = pt::read<uint16>(s);
            log((boost::format("Name: '%1%', parent: '%2$x'") % m_name % m_hierarchy_father).str());
        }
        else if (info.is(CHUNK_KF_OBJECT_PIVOT_POINT))
        {
            m_pivot_point = pt::read<vector3f>(s);
            log((boost::format("Pivot point '%1%'") % pt::to_string(m_pivot_point)).str());
        }    
        /*else if (info.is(CHUNK_KF_POSITION_TRACK))
        {
            track(info, s);
        }*/   
        else if (info.id >= CHUNK_KF_POSITION_TRACK
            && info.id <= CHUNK_KF_HIDE_TRACK)
        {
			m_tracks.push_back(boost::shared_ptr<track>(new track(info, s)));
        }    
        else
        {
            log((boost::format("Skipping unknown track: %1$x") % info.id).str());
            int arr=1;arr;
        }

        skip_chunk(info, s);
    }
}

std::string const& keyframer_node::name() const
{
    return m_name;
}

unsigned short keyframer_node::hierarchy_position() const
{
    return m_hierarchy_position;
}

unsigned short keyframer_node::hierarchy_father() const
{
    return m_hierarchy_father;
}

unsigned short keyframer_node::obj_flag1() const
{
    return m_obj_flag1;
}

unsigned short keyframer_node::obj_flag2() const
{
    return m_obj_flag2;
}

vector3f const& keyframer_node::pivot_point() const
{
    return m_pivot_point;
}

keyframer_node::track_container const& keyframer_node::tracks() const
{
    return m_tracks;
}

bool keyframer_node::is_target() const
{
    return m_is_target;
}

// keyframer

keyframer::keyframer()
{
}

void keyframer::read(chunk_info const& parent_info, bounded_input_stream& s)
{
    // Jump start allocations.
    m_nodes.reserve(10);

    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_KF_AMBIENT_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s)));
        }
        else if (info.is(CHUNK_KF_MESH_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s)));
        }
        else if (info.is(CHUNK_KF_CAMERA_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s)));
        }
        else if (info.is(CHUNK_KF_CAMERA_TARGET_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s, true)));
        }
        else if (info.is(CHUNK_KF_OMNI_LIGHT_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s)));
        }
        else if (info.is(CHUNK_KF_SPOTLIGHT_TARGET_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s, true)));
        }
        else if (info.is(CHUNK_KF_SPOTLIGHT_INFO))
        {
            m_nodes.push_back(node_type(new keyframer_node(info, s)));
        }
        else if (info.is(CHUNK_FRAMES))
        {
            m_frame_start = pt::read<uint32>(s);
            m_frame_end = pt::read<uint32>(s);
            log(format("Frame start: '%1%', end: '%2%'") % m_frame_start % m_frame_end);
        }
        else
        {
            int arr = 1; arr;
        }

        skip_chunk(info, s);
    }
}

int keyframer::frame_start() const
{
    return m_frame_start;
}

int keyframer::frame_end() const
{
    return m_frame_end;
}

keyframer::node_container const& keyframer::nodes() const
{
    return m_nodes;
}

boost::shared_ptr<keyframer> read_keyframer(chunk_info const& parent_info,
    bounded_input_stream& s)
{
	boost::shared_ptr<keyframer> kf(new keyframer);
    kf->read(parent_info, s);
    return kf;
}

}}} // namespace pt::conversion::file_3ds
