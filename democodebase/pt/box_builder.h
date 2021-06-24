#ifndef PT_BOX_BUILDER_H
#define PT_BOX_BUILDER_H

#include "box.h"
//#include "boost/optional.h"
#include <boost/numeric/conversion/bounds.hpp>

namespace pt {

namespace numeric = ::boost::numeric;

/// Builds a bounding box.
template<typename T, int dim>
class box_builder
{
public:
    box_builder();

    void add(vector<T,dim> const& position);

    bool empty() const;

    /// Returns box enclosing all added positions. Returns nothing if no
    /// positions are added.
    box<T,dim> bounding_box() const;

private: // data members
    bool m_empty;
	vector<T,dim> m_min;
	vector<T,dim> m_max;
};

/// 3 dimension float box. \relatesalso box
typedef box_builder<float,3> box_builder3f;

template<typename T, int dim>
inline box_builder<T,dim>::box_builder()
:   m_empty(true)
,   m_min(vector<T,dim>::uninitialized)
,   m_max(vector<T,dim>::uninitialized)
{
    for (size_t i = 0; i < dim; ++i)
    {
        m_min[i] = numeric::bounds<T>::highest();
        m_max[i] = numeric::bounds<T>::lowest();
    }
}

template<typename T, int dim>
inline void box_builder<T,dim>::add(vector<T,dim> const& position)
{
    m_empty = false;

    for (size_t i = 0; i < dim; ++i)
    {
        m_min[i] = std::min(m_min[i], position[i]);
        m_max[i] = std::max(m_max[i], position[i]);
    }
}

template<typename T, int dim>
inline bool box_builder<T,dim>::empty() const
{
    return m_empty;
}

template<typename T, int dim>
inline box<T,dim> box_builder<T,dim>::bounding_box() const
{
    return box_from_min_max(m_min, m_max);
    //return m_empty
    //    ? optional<box<T,dim> >()
    //    : optional<box<T,dim> >(box_from_min_max(m_min, m_max));
}

} // namespace pt

#endif
