#include "attribute_collection.h"
#include "pt/any_error.h"
#include "pt/format.h"

namespace pt { namespace d3d9 { namespace rendering {

size_t attribute_collection::attribute_count() const
{
    return m_attributes.size();
}

void attribute_collection::add(semantic const& semantic,
	std::shared_ptr<source::vertex_attribute> attribute)
{
    m_attributes.push_back(std::make_pair(semantic, attribute));
}

attribute_collection::const_iterator attribute_collection::begin() const
{
    return m_attributes.begin();
}

attribute_collection::const_iterator attribute_collection::end() const
{
    return m_attributes.end();
}

std::shared_ptr<source::vertex_attribute const> attribute_collection::attribute(
    semantic const& semantic) const
{
    for (const_iterator i = begin(); i != end(); ++i)
    {
        if (semantic == i->first)
            return i->second;
    }
    throw any_error(format("attribute '%1%' not found") % semantic.text());
}

}}} // namespace pt::d3d9::rendering
