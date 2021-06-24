#include "item_collection.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

item_collection::const_iterator item_collection::begin() const
{
    return m_children.begin();
}

item_collection::const_iterator item_collection::end() const
{
    return m_children.end();
}

item_collection::iterator item_collection::begin()
{
    return m_children.begin();
}

item_collection::iterator item_collection::end()
{
    return m_children.end();
}

void item_collection::push_back(std::shared_ptr<item> child)
{
    m_children.push_back(move(child));
}

void item_collection::erase(std::shared_ptr<item> const& child)
{
    iterator i = find(child);
    assert(i != m_children.end());
    m_children.erase(i);
}

item_collection::iterator item_collection::find(std::shared_ptr<item> const& child)
{
    return std::find(m_children.begin(), m_children.end(), child);
}

}}} // namespace pt::d3d9::rendering
