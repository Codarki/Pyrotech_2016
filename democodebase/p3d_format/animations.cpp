#include "animations.h"
#include "chunk_reader.h"
#include "pt/any_error.h"
#include "pt/log.h"
#include "pt/vector_utility.h"
#include "pt/format.h"

namespace p3d_format {

key_data::key_data()
:   tension(0)
,   continuity(0)
,   bias(0)
,   ease_to(0)
,   ease_from(0)
{
}

animation::animation(chunk_reader& chunk)
{
    m_object_name = chunk.read<pt::string_wrapper>().string();
    m_is_target = chunk.read<bool>();
    m_hierarchy_position = chunk.read<int16>();
    m_hierarchy_parent = chunk.read<int16>();
    m_pivot = chunk.read<vector3f>();

    //log(format("Reading animations for \"%1%\": position %2%, parent %3%")
    //    % m_object_name % m_hierarchy_position % m_hierarchy_parent);

    //assert(m_hierarchy_parent == -1);

    size_t track_count = chunk.read<uint8>();

    log(format("Loading animation for %1% - Track count: %2%") % m_object_name % track_count);

    //log("Pivot: ("
    //    + lexical_cast<std::string>(static_cast<float>(m_pivot[0]))
    //    + "," + lexical_cast<std::string>(static_cast<float>(m_pivot[1]))
    //    + "," + lexical_cast<std::string>(static_cast<float>(m_pivot[2]))
    //    + ")");

    for (size_t j = 0; j < track_count; ++j)
    {
        uint8 track_type = chunk.read<uint8>();
        uint16 track_flag = chunk.read<uint16>();
        uint32 key_count = chunk.read<uint32>();

        if (track_flag != 0)
            log("unimplemented animation track flag");

        switch(track_type)
        {
        case 0:
            log(format(" Position track key count: %1%") % key_count);
            read_position_track(chunk, key_count);
            break;

        case 1:
            log(format(" Rotation track key count: %1%") % key_count);
            read_rotation_track(chunk, key_count);
            break;

        case 2:
            log(format(" Scale track key count: %1%") % key_count);
            read_scale_track(chunk, key_count);
            break;

        case 3:
            log(format(" FOV track key count: %1%") % key_count);
            read_float_track(chunk, key_count, m_fov);
            
            typedef  pt::math::kb_data_set<float>::const_iterator anim_iterator;
            
            for (anim_iterator i = m_fov.begin(); i != m_fov.end(); ++i)
            {
                log(format("  Time: %1%, Value: %2%") % i->time() % i->value());	
            }
            break;

        case 4:
            log(format(" ROLL track key count: %1%") % key_count);
            read_float_track(chunk, key_count, m_roll);
            typedef  pt::math::kb_data_set<float>::const_iterator anim_iterator;
            
            for (anim_iterator i = m_roll.begin(); i != m_roll.end(); ++i)
            {
                log(format("  Time: %1%, Value: %2%") % i->time() % i->value());	
            }
            break;

        case 5:
            log("Color track not implemented");
            break;

        case 7:
            log(format(" Hotspot track key count: %1%") % key_count);
            read_float_track(chunk, key_count, m_hotspot);
            break;

        case 8:
            log(format(" Falloff track key count: %1%") % key_count);
            read_float_track(chunk, key_count, m_falloff);
            break;

        case 9:
            log(format(" Visibility track key count: %1%") % key_count);
            read_float_track(chunk, key_count, m_hide);
            
            typedef  pt::math::kb_data_set<float>::const_iterator anim_iterator;
            
            for (anim_iterator i = m_hide.begin(); i != m_hide.end(); ++i)
            {
                log(format("  Time: %1%, Value: %2%") % i->time() % i->value());	
            }
            
            break;

        default:
            log(format("unimplemented animation track %1% found") % static_cast<uint16>(track_type));
            assert(false);
        }
    }
}

std::string const& animation::object_name() const
{
    return m_object_name;
}

bool animation::is_target() const
{
    return m_is_target;
}

void animation::set_parent(boost::shared_ptr<animation> parent)
{
    m_parent = parent;
}

int16 animation::hierarchy_position() const
{
    return m_hierarchy_position;
}

int16 animation::hierarchy_parent() const
{
    return m_hierarchy_parent;
}

vector3f animation::pivot() const
{
    return m_pivot;
}


math::kb_data_set<float> const* animation::position_data() const
{
    return m_position;
}

math::kb_data_set<float> const* animation::rotation_data() const
{
    return m_rotation;
}

math::kb_data_set<float> const* animation::scaling_data() const
{
    return m_scale;
}

math::kb_data_set<float> const& animation::falloff_data() const
{
    return m_falloff;
}

math::kb_data_set<float> const& animation::fov_data() const
{
    return m_fov;
}

math::kb_data_set<float> const& animation::roll_data() const
{
    return m_roll;
}

math::kb_data_set<float> const& animation::hide_data() const
{
    return m_hide;
}

namespace {
    math::kochanek_bartels_parameters kb_parameters(key_data const& data)
    {
        return math::kochanek_bartels_parameters(
            data.tension, data.bias, data.continuity);
    }
}

void animation::read_position_track(chunk_reader& chunk, size_t key_count)
{
    math::data_set<float> position[3];
    std::vector<math::kochanek_bartels_parameters> parameters;

    parameters.reserve(key_count);
    position[0].reserve(key_count);
    position[1].reserve(key_count);
    position[2].reserve(key_count);

    for (size_t i = 0; i < key_count; ++i)
    {
        key_data data = read_acceleration_data(chunk);

        uint32 key_number = chunk.read<uint32>();

        // TODO need that fps parameter here.
        float time = static_cast<float>(key_number);

        float values[3];
        chunk.read_array<float32>(3, values);

        math::data_set<float>::iterator iter = position[0].add(time, values[0]);
        if (iter != position[0].end())
        {
            // Ignore return values, they're not end().
            iter = position[1].add(time, values[1]);
            iter = position[2].add(time, values[2]);
            parameters.push_back(kb_parameters(data));
        }
    }

    m_position[0] = math::kb_data_set<float>(position[0], parameters);
    m_position[1] = math::kb_data_set<float>(position[1], parameters);
    m_position[2] = math::kb_data_set<float>(position[2], parameters);
}

void animation::read_rotation_track(chunk_reader& chunk, size_t key_count)
{
    //size_t bytes = key_count * sizeof(float) * 4 + sizeof(uint32);
    //log(format("Reading rotation track: %1% keys, %2% bytes")
    //    % key_count % bytes);

    math::data_set<float> rotation[4];
    std::vector<math::kochanek_bartels_parameters> parameters;

    parameters.reserve(key_count);
    rotation[0].reserve(key_count);
    rotation[1].reserve(key_count);
    rotation[2].reserve(key_count);
    rotation[3].reserve(key_count);

    for (size_t i = 0; i < key_count; ++i)
    {
        key_data data = read_acceleration_data(chunk);

        uint32 key_number = chunk.read<uint32>();

        // TODO need that fps parameter here.
        float time = static_cast<float>(key_number);

        vector4f axis_angle(chunk.read<vector4f>());

        //log(boost::format("Rotation: time (angle,axis), %1% (%2%, %3%, %4%, %5%)")
        //    % time % axis_angle[0] % axis_angle[1] % axis_angle[2] % axis_angle[3]);

        math::data_set<float>::iterator iter = rotation[0].add(time, axis_angle[0]);
        if (iter != rotation[0].end())
        {
            // Ignore return values, they're not end().
            iter = rotation[1].add(time, axis_angle[1]);
            iter = rotation[2].add(time, axis_angle[2]);
            iter = rotation[3].add(time, axis_angle[3]);
            parameters.push_back(kb_parameters(data));
        }
    }

    m_rotation[0] = math::kb_data_set<float>(rotation[0], parameters);
    m_rotation[1] = math::kb_data_set<float>(rotation[1], parameters);
    m_rotation[2] = math::kb_data_set<float>(rotation[2], parameters);
    m_rotation[3] = math::kb_data_set<float>(rotation[3], parameters);
}

void animation::read_scale_track(chunk_reader& chunk, size_t key_count)
{
    math::data_set<float> scale[3];
    std::vector<math::kochanek_bartels_parameters> parameters;

    parameters.reserve(key_count);
    scale[0].reserve(key_count);
    scale[1].reserve(key_count);
    scale[2].reserve(key_count);

    for (size_t i = 0; i < key_count; ++i)
    {
        key_data data = read_acceleration_data(chunk);

        uint32 key_number = chunk.read<uint32>();

        // TODO need that fps parameter here.
        float time = static_cast<float>(key_number);

        float values[3];
        chunk.read_array<float32>(3, values);

        math::data_set<float>::iterator iter = scale[0].add(time, values[0]);
        if (iter != scale[0].end())
        {
            // Ignore return values, they're not end().
            scale[1].add(time, values[1]);
            scale[2].add(time, values[2]);
            parameters.push_back(kb_parameters(data));
        }
    }

    m_scale[0] = math::kb_data_set<float>(scale[0], parameters);
    m_scale[1] = math::kb_data_set<float>(scale[1], parameters);
    m_scale[2] = math::kb_data_set<float>(scale[2], parameters);
}

void animation::read_float_track(chunk_reader& chunk, size_t key_count,
    math::kb_data_set<float>& target)
{
    assert(target.size() == 0);

    math::data_set<float> values;
    std::vector<math::kochanek_bartels_parameters> parameters;

    parameters.reserve(key_count);
    values.reserve(key_count);


    for (size_t i = 0; i < key_count; ++i)
    {
        key_data data = read_acceleration_data(chunk);

        uint32 key_number = chunk.read<uint32>();

        // TODO need that fps parameter here.
        float time = static_cast<float>(key_number);
        float value = chunk.read<float32>();

        math::data_set<float>::iterator iter = values.add(time, value);
        if (iter != values.end())
        {
            parameters.push_back(kb_parameters(data));
        }
    }

    target = math::kb_data_set<float>(values, parameters);
}

key_data animation::read_acceleration_data(chunk_reader& chunk)
{
    key_data result;
    uint16 key_acceleration_data = chunk.read<uint16>();

    for (size_t i = 0; i < 5; ++i)
    {
        if ((key_acceleration_data & (1 << i)) == 0)
            continue;

        switch(i)
        {
        case 0:
            result.tension = chunk.read<float32>();
            break;
        case 1:
            result.continuity = chunk.read<float32>();
            break;
        case 2:
            result.bias = chunk.read<float32>();
            break;
        case 3:
            result.ease_to = chunk.read<float32>();
            break;
        case 4:
            result.ease_from = chunk.read<float32>();
            break;
        }
    }
    return result;
}

namespace {
	boost::shared_ptr<animation> find_animation(
		std::vector<boost::shared_ptr<animation> >& animations, uint16 position)
    {
		typedef std::vector<boost::shared_ptr<animation> > container;

        for (container::iterator i = animations.begin(),
            i_end = animations.end(); i != i_end; ++i)
        {
            if ((*i)->hierarchy_position() == position)
                return *i;
        }
        throw any_error("animation not found");
    }
} // anonymous namespace

void set_parent_hierarchy(std::vector<boost::shared_ptr<animation> >& animations)
{
	typedef std::vector<boost::shared_ptr<animation> > container;

    for (container::iterator i = animations.begin(),
        i_end = animations.end(); i != i_end; ++i)
    {
        uint16 const parent_position = (*i)->hierarchy_parent();
        if (parent_position == 0xffff)
            continue;

		boost::shared_ptr<animation> parent
            = find_animation(animations, parent_position);

        (*i)->set_parent(parent);
    }
}

} // namespace p3d_format
