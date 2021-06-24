#ifndef PT_D3D9_SHADERS_STREAM_COLLECTION_H
#define PT_D3D9_SHADERS_STREAM_COLLECTION_H

#include "pt/config_node_fwd.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace shaders {

/// One vertex stream in hlsl shader.
class stream
{
public:
    stream(config_node const& config);

private: // type definitions
    struct element
    {
        std::string semantic;
        size_t index;
    };

private: // data members
    std::string m_name;
    std::vector<element> m_elements;
};

/// Collection of vertex streams in hlsl shader.
class stream_collection
{
public:
    void read(config_node const& config);

    std::vector<stream> const& container() const;

private: // data members
    std::vector<stream> m_streams;
};

}}} // namespace pt::d3d9::shaders

#endif
