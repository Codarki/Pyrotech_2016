#ifndef PT_D3D9_RENDERING_ATTRIBUTE_COLLECTION_H
#define PT_D3D9_RENDERING_ATTRIBUTE_COLLECTION_H

#include "../semantic.h"
#include "pt/vector.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

namespace source {
class vertex_attribute;
}

// Maps vertex semantics and attributes together.
class attribute_collection
{
private: // type definitions
    typedef std::pair<semantic,std::shared_ptr<source::vertex_attribute> > entry;
    typedef std::vector<entry> container;
public:
    typedef container::const_iterator const_iterator;

public:
    size_t attribute_count() const;

    void add(semantic const& semantic,
        std::shared_ptr<source::vertex_attribute> attribute);

    const_iterator begin() const;
    const_iterator end() const;

    std::shared_ptr<source::vertex_attribute const> attribute(
        semantic const& semantic) const;

private: // data members
    container m_attributes;
};

}}} // namespace pt::d3d9::rendering

#endif
