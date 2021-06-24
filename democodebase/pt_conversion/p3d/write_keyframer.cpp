#include "../file_3ds/chunk_ids.h"
#include "../file_3ds/keyframer.h"
#include "../file_3ds/key.h"
#include "pt/any_error.h"
#include "pt/file_output_stream.h"
#include "pt/vector_utility.h"
#include "pt/log.h"
#include "pt/boost/cast.h"
#include "pt/boost/smart_ptr.h"
#include "p3d_format/chunk_writer.h"
#include "p3d_format/id.h"

namespace pt { namespace conversion { namespace p3d {

namespace {
    vector3f rh_to_lh(vector3f const& v)
    {
        return vector3f(v[0], v[2], v[1]);
        //return vector3f(v[0], v[1], v[2]);
    }

    enum {
        position_track = 0,
        rotation_track,
        scale_track,
        fov_track,   
        roll_track,
        color_track,   
        morph_track,  
        hotspot_track,
        falloff_track,
        hide_track,
    };

    uint8 track_id(int track_type)
    {
        switch(track_type)
        {
            case file_3ds::CHUNK_KF_POSITION_TRACK:
                return position_track;
            case file_3ds::CHUNK_KF_ROTATION_TRACK:
                return rotation_track;
            case file_3ds::CHUNK_KF_SCALE_TRACK:
                return scale_track;
            case file_3ds::CHUNK_KF_FOV_TRACK:
                return fov_track;
            case file_3ds::CHUNK_KF_ROLL_TRACK:
                return roll_track;
            case file_3ds::CHUNK_KF_COLOR_TRACK:
                return color_track;
            case file_3ds::CHUNK_KF_MORPH_TRACK:
                return morph_track;
            case file_3ds::CHUNK_KF_HOTSPOT_TRACK:
                return hotspot_track;
            case file_3ds::CHUNK_KF_FALLOFF_TRACK:
                return falloff_track;
            case file_3ds::CHUNK_KF_HIDE_TRACK:
                return hide_track;
        };
        throw any_error("unknown 3ds track type");
    }

    void write_key(int track_type, file_3ds::key const& key,
        file_output_stream& s)
    {
        s; track_type;

        file_3ds::typed_key_vector3f const* key_vector3f =
            dynamic_cast<file_3ds::typed_key_vector3f const*>(&key);

        file_3ds::typed_key_axis_angle const* key_axis_angle =
            dynamic_cast<file_3ds::typed_key_axis_angle const*>(&key);

        file_3ds::typed_key_float const* key_float =
            dynamic_cast<file_3ds::typed_key_float const*>(&key);

        write(s, static_cast<uint32>(key.key_number()));

        switch(track_type)
        {
            case file_3ds::CHUNK_KF_POSITION_TRACK:
                write(s, vector3f(rh_to_lh(key_vector3f->value())));
                break;

            case file_3ds::CHUNK_KF_ROTATION_TRACK:
                write(s, float(key_axis_angle->value().angle));
                write(s, vector3f(rh_to_lh(key_axis_angle->value().axis)));
                //write(s, vector3f(key_axis_angle->value().axis));
                break;

            case file_3ds::CHUNK_KF_SCALE_TRACK:
                write(s, vector3f(rh_to_lh(key_vector3f->value())));
                break;

            case file_3ds::CHUNK_KF_FOV_TRACK:
                write(s, key_float->value());
                break;

            case file_3ds::CHUNK_KF_ROLL_TRACK:
                write(s, key_float->value());
                break;

            case file_3ds::CHUNK_KF_COLOR_TRACK:
                log("Color track not implemented");
                break; // TODO

            case file_3ds::CHUNK_KF_MORPH_TRACK:
                log("Morph track not implemented");
                break; // TODO ascii track

            case file_3ds::CHUNK_KF_HOTSPOT_TRACK:
                write(s, key_float->value());
                break;

            case file_3ds::CHUNK_KF_FALLOFF_TRACK:
                write(s, key_float->value());
                break;

            case file_3ds::CHUNK_KF_HIDE_TRACK:
                //log("Hide track not implemented");
				write(s, key_float->value());
                break; // TODO none trac
        };
    }
} // anonymous namespace

uint16 acceleration_bitfield(
    file_3ds::key::acceleration_params const& acceleration)
{
    uint16 flags = 0;

    if (acceleration.tension != 0.0f)
        flags |= 1 << 0;
    if (acceleration.continuity != 0.0f)
        flags |= 1 << 1;
    if (acceleration.bias != 0.0f)
        flags |= 1 << 2;
    if (acceleration.ease_to != 0.0f)
        flags |= 1 << 3;
    if (acceleration.ease_from != 0.0f)
        flags |= 1 << 4;

    return flags;
}

void write_keyframer(file_3ds::keyframer const& keyframer,
    file_output_stream& s)
{
    p3d_format::chunk_writer chunk(s, p3d_format::id_animation);

    chunk.write<uint32>(keyframer.frame_start());
    chunk.write<uint32>(keyframer.frame_end());

    file_3ds::keyframer::node_container const& nodes = keyframer.nodes();
    chunk.write(static_cast<uint32>(nodes.size()));

    for (auto const& node_ptr : nodes)
    {
        file_3ds::keyframer_node const& node = *node_ptr;

        chunk.write<std::string>(node.name());
        chunk.write<bool>(node.is_target());
        chunk.write<int16>(node.hierarchy_position());
        chunk.write<int16>(node.hierarchy_father());
        chunk.write<vector3f>(rh_to_lh(node.pivot_point()));
        chunk.write(static_cast<int8>(node.tracks().size()));

        for (auto const& track_ptr : node.tracks())
        {
            file_3ds::track const& track = *track_ptr;

            chunk.write<uint8>(track_id(track.type()));
            chunk.write<uint16>(track.flag());
            chunk.write<uint32>(numeric_cast<uint32>(track.keys().size()));

            for (auto const& key_ptr : track.keys())
            {
                file_3ds::key const& key = *key_ptr;

                chunk.write<uint16>(
                    acceleration_bitfield(key.acceleration()));

                // TODO write acceleration data
                //chunk.write<uint16>(0);

                if (key.acceleration().tension != 0.0f)
                    chunk.write<float32>(key.acceleration().tension);
                if (key.acceleration().continuity != 0.0f)
                    chunk.write<float32>(key.acceleration().continuity);
                if (key.acceleration().bias != 0.0f)
                    chunk.write<float32>(key.acceleration().bias);
                if (key.acceleration().ease_to != 0.0f)
                    chunk.write<float32>(key.acceleration().ease_to);
                if (key.acceleration().ease_from != 0.0f)
                    chunk.write<float32>(key.acceleration().ease_from);

                // key data:
                write_key(track.type(), key, s);
            }
        }
    }
}

}}} // namespace pt::conversion::p3d
