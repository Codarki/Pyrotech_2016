#include "corner_access.h"
#include "corner_collection.h"

namespace p3d_format {

// corner_access

corner_access::corner_access(corner_collection& collection, size_t index)
: m_collection(&collection)
, m_index(index)
{
}

corner_access::corner_access(corner_access&& other)
: m_collection(other.m_collection)
, m_index(other.m_index)
{
	other.m_collection = nullptr;
	other.m_index = 0;
}

corner_access::corner_access(corner_access const& other)
: m_collection(other.m_collection)
, m_index(other.m_index)
{
}

corner_access& corner_access::operator=(corner_access&& rhs)
{
	m_collection = rhs.m_collection;
	m_index = rhs.m_index;
	rhs.m_collection = nullptr;
	rhs.m_index = 0;
	return *this;
}

corner& corner_access::get()
{
    return m_collection->corner_at(m_index);
}

corner const& corner_access::get() const
{
    return m_collection->corner_at(m_index);
}

} // namespace p3d_format
