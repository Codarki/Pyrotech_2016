#include "data.h"
#include "common.h"
#include "object.h"
#include "material.h"
#include "keyframer.h"
#include "pt/bounded_input_stream.h"
#include "pt/log.h"

namespace pt { namespace conversion { namespace file_3ds {

data::data()
{
}

void data::read(chunk_info const& parent_info, bounded_input_stream& s)
{
    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_OBJMESH))
        {
            // call recursively again
            read(info, s);
        }
        else if (info.is(CHUNK_MATERIAL))
        {
            m_materials.push_back(read_material(info, s));
        }
        else if (info.is(CHUNK_KEYFRAMER))
        {
            m_keyframer = read_keyframer(info, s);
        }
        //else if (info.is(CHUNK_ONE_UNIT))
        //{
        //    log("UNITTI");
        //    float unit = pt::read<float>(s);
        //    unit;
        //}

        if (info.is(CHUNK_OBJBLOCK))
        {
            auto obj = read_object(info, s);
            if (obj)
                m_objects.push_back(obj);
        }

        skip_chunk(info, s);
    }
}

data::object_container const& data::objects() const
{
    return m_objects;
}

data::material_container const& data::materials() const
{
    return m_materials;
}

boost::shared_ptr<file_3ds::keyframer const> data::keyframer() const
{
    return m_keyframer;
}

size_t data::frame_count() const
{
    assert(m_keyframer);
    return m_keyframer->frame_end() - m_keyframer->frame_start();
}


}}} // namespace pt::conversion::file_3ds
