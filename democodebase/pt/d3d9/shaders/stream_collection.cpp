#include "stream_collection.h"
#include "pt/config_node.h"
#include "pt/boost/assert.h"

namespace pt { namespace d3d9 { namespace shaders {

// stream

stream::stream(config_node const& config)
{
    m_name = config.key();
    assert(m_name.compare(0, 6, "stream") == 0);

    for (config_node::const_iterator i = config.begin();
        i != config.end(); ++i)
    {
        element e;
        e.semantic = i->key();
        i->parse(e.index);

        m_elements.push_back(e);
    }
}

// stream_collection

void stream_collection::read(config_node const& config)
{
    assert(config.key() == "streams");
    for (config_node::const_iterator i = config.begin(); i != config.end(); ++i)
    {
        m_streams.push_back(stream(*i));
    }
}

std::vector<stream> const& stream_collection::container() const
{
    return m_streams;
}

}}} // namespace pt::d3d9::shaders
