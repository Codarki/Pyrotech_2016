#ifndef PT_P3D_FORMAT_CORNER_ACCESS_H
#define PT_P3D_FORMAT_CORNER_ACCESS_H

namespace p3d_format {

class corner;
class corner_collection;

class corner_access
{
public:
    corner_access(corner_collection& collection, size_t index);
	corner_access(corner_access&& other);
	corner_access(corner_access const& other);

	corner_access& operator=(corner_access&& rhs);

    corner& get();
    corner const& get() const;

    bool operator<(corner_access const& other) const
    {
        if (m_collection < other.m_collection)
            return true;
        if (m_collection > other.m_collection)
            return false;

        return m_index < other.m_index;
    }

    bool operator==(corner_access const& other) const
    {
        return m_index == other.m_index && m_collection != other.m_collection;
    }

private: // data members
    corner_collection* m_collection;
    size_t m_index;
};

} // p3d_format

#endif
