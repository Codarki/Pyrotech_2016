#ifndef PT_D3D9_RENDERING_COMPOSITE_CURVE_DESCRIPTION_COLLECTION_H
#define PT_D3D9_RENDERING_COMPOSITE_CURVE_DESCRIPTION_COLLECTION_H

#include "pt/std/vector.h"
#include "pt/std/map.h"

namespace pt { namespace d3d9 { namespace rendering {

class composite_curve_description_collection
{
public:
    void add(std::string const& name, std::string const& curve_name)
    {
        m_container[name].push_back(curve_name);
    }

private: // data members
    typedef std::vector<std::string> map_value_type;
    std::map<std::string,map_value_type> m_container;
};

}}} // namespace pt::d3d9::rendering

#endif
