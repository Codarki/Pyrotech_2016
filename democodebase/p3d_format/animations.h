#ifndef PT_P3D_FORMAT_ANIMATIONS_H
#define PT_P3D_FORMAT_ANIMATIONS_H

#include "pt/basic_types.h"
#include "pt/vector.h"
#include "pt/boost/shared_ptr.h"
#include "pt/math/kb_data_set.h"
#include "pt/std/string.h"

namespace p3d_format {

using namespace pt;

class chunk_reader;

struct key_data
{
    key_data();

    float tension;
    float continuity;
    float bias;
    float ease_to;
    float ease_from;
};

class animation
{
public:
    animation(chunk_reader& chunk);

    std::string const& object_name() const;
    bool is_target() const;
	void set_parent(boost::shared_ptr<animation> parent);

    int16 hierarchy_position() const;
    int16 hierarchy_parent() const;
    vector3f   pivot() const;

    math::kb_data_set<float> const* position_data() const;
    math::kb_data_set<float> const* rotation_data() const;
    math::kb_data_set<float> const* scaling_data() const;
    math::kb_data_set<float> const& falloff_data() const;
    math::kb_data_set<float> const& fov_data() const;
    math::kb_data_set<float> const& roll_data() const;
	math::kb_data_set<float> const& hide_data() const;

private: // helper functions
    void read_position_track(chunk_reader& chunk, size_t key_count);
    void read_rotation_track(chunk_reader& chunk, size_t key_count);
    void read_scale_track(chunk_reader& chunk, size_t key_count);
    void read_float_track(chunk_reader& chunk, size_t key_count,
        math::kb_data_set<float>& target);
    key_data read_acceleration_data(chunk_reader& chunk);

private: // data members
    std::string m_object_name;
    bool m_is_target;
    int16 m_hierarchy_position;
    int16 m_hierarchy_parent;
    vector3f m_pivot;

	boost::shared_ptr<animation> m_parent;

    math::kb_data_set<float> m_position[3];
    math::kb_data_set<float> m_rotation[4];
    math::kb_data_set<float> m_scale[3];
    math::kb_data_set<float> m_fov;
    math::kb_data_set<float> m_roll;
    math::kb_data_set<float> m_hotspot;
    math::kb_data_set<float> m_falloff;
	math::kb_data_set<float> m_hide;
};

/// Sets the parent for all \a animations.
void set_parent_hierarchy(std::vector<boost::shared_ptr<animation> >& animations);

} // namespace p3d_format

#endif
