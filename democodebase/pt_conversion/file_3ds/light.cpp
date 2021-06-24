#include "light.h"
#include "common.h"
#include "pt/log.h"
#include "pt/bounded_input_stream.h"
#include "pt/vector_utility.h"
#include "pt/string_wrapper.h"
#include "pt/boost/format.h"

namespace pt { namespace conversion { namespace file_3ds {

light::light(std::string const& name)
:   object(name)
,   m_is_spotlight(false)
,   m_hotspot(0)
,   m_falloff(0)
{
}

void light::read(chunk_info const& parent_info, bounded_input_stream& s)
{
    m_position = pt::read<vector3f>(s);

    while(s.position() < parent_info.end_offset)
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_SPOTLIGHT))
        {
            log((boost::format("spotlight (%1$x)") % info.id).str());
            m_is_spotlight = true;
            m_target = pt::read<vector3f>(s);
            m_hotspot = pt::read<float>(s);
            m_falloff = pt::read<float>(s);
            logging::scoped_indent indent;
            log((boost::format("hotspot %1%, falloff %2%") % m_hotspot % m_falloff).str());
        }
        else if (info.is(CHUNK_ATTENUATION_ON))
        {
            log((boost::format("attenuation on (%1$x) ignored") % info.id).str());
        }
        else if (info.is(CHUNK_RANGE_START))
        {
            log((boost::format("range start (%1$x)") % info.id).str());
            m_range[0] = pt::read<float>(s);
            logging::scoped_indent indent;
            log((boost::format("start %1%") % m_range[0]).str());
        }
        else if (info.is(CHUNK_RANGE_END))
        {
            log((boost::format("range end (%1$x)") % info.id).str());
            m_range[1] = pt::read<float>(s);
            logging::scoped_indent indent;
            log((boost::format("end %1%") % m_range[1]).str());
        }
        else if (info.is(CHUNK_MULTIPLIER))
        {
            log((boost::format("multiplier (%1$x)") % info.id).str());
            m_multiplier = pt::read<float>(s);
            logging::scoped_indent indent;
            log((boost::format("multiplier %1%") % m_multiplier).str());
        }
        else if (info.is(CHUNK_RGBF))
        {
            //log(format("color (%1$x)") % info.id);
            m_color = pt::read<vector3f>(s);
        }
        else if (info.is(CHUNK_RGBB))
        {
            //log(format("color (%1$x)") % info.id);
            for (size_t i = 0; i < 3; ++i)
            {
                m_color[i] = pt::read<uint8>(s) / 255.0f;
            }
        }
        else
        {
            log((boost::format("unknown (%1$x)") % info.id).str());
        }
        skip_chunk(info, s);
    }

/*
   fread(&Lights[Lights.length()-1].posdata,3*sizeof(float),1,handle);
   Lights[Lights.length()-1].RGB = false;
   Lights[Lights.length()-1].RGB24 = false;

   i = 3*sizeof(float);
   while(i<len)
   {
     tmp = ReadChunkHdr(handle);
     if (tmp.ID == 0x0010) ReadRgb(handle,&Lights[Lights.length()-1]);
     else if (tmp.ID == 0x0011) ReadRgb24(handle,&Lights[Lights.length()-1]);
     else Skip3DSChunk(handle,tmp.Len);
     i+=(tmp.Len+6);
   }
*/
}

vector3f const& light::position() const
{
    return m_position;
}

vector3f const& light::color() const
{
    return m_color;
}

bool light::is_spotlight() const
{
    return m_is_spotlight;
}

vector3f const& light::target() const
{
    assert(is_spotlight());
    return m_target;
}

float light::hotspot() const
{
    assert(is_spotlight());
    return m_hotspot;
}

float light::falloff() const
{
    assert(is_spotlight());
    return m_falloff;
}

vector2f const& light::range() const
{
    return m_range;
}

float light::multiplier() const
{
    return m_multiplier;
}

boost::shared_ptr<light> read_light(chunk_info const& parent_info,
    std::string const& name, bounded_input_stream& s)
{
	boost::shared_ptr<light> l(new light(name));
    l->read(parent_info, s);
    return l;
}

}}} // namespace pt::conversion::file_3ds
