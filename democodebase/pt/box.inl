#ifndef PT_BOX_INL
#define PT_BOX_INL

namespace pt {

template<typename T, int dim>
box<T,dim>::box()
{
}

template<typename T, int dim>
box<T,dim>::box(enum uninitialized)
:   m_center(vector<T,dim>::uninitialized)
,   m_size(vector<T,dim>::uninitialized)
{
}

template<typename T, int dim>
box<T,dim>::box(vector<T,dim> const& center, vector<T,dim> const& size)
:   m_center(center)
,   m_size(size)
{
}

template<typename T, int dim>
template<typename U>
box<T,dim>::box(box<U,dim> const& other)
:   m_center(static_vector_cast<T>(other.center()))
,   m_size(static_vector_cast<T>(other.size()))
{
}

template<typename T, int dim>
vector<T,dim> const& box<T,dim>::center() const
{
    return m_center;
}

template<typename T, int dim>
void box<T,dim>::set_center(vector<T,dim> const& position)
{
    m_center = position;
}

template<typename T, int dim>
vector<T,dim> const& box<T,dim>::size() const
{
    return m_size;
}

template<typename T, int dim>
void box<T,dim>::set_size(vector<T,dim> const& size)
{
    m_size = size;
}

template<typename T, int dim>
vector<T,dim> box<T,dim>::half_axis_lengths() const
{
    return m_size / static_cast<T>(2);
}

// Free functions.

template<typename T, int dim>
box<T,dim> box_from_min_max(vector<T,dim> const& min,
    vector<T,dim> const& max)
{
    vector<T,dim> center((min + max) / static_cast<T>(2));
    vector<T,dim> size(max - min);
    return box<T,dim>(center, size);
}

template<typename T, int dim>
T half_extent(box<T,dim> const& box, size_t axis)
{
    return box.half_axis_lengths()[axis];
}

template<typename T, int dim>
T min_axis(box<T,dim> const& box, size_t axis)
{
    return box.center()[axis] - half_extent(box, axis);
}

template<typename T, int dim>
T max_axis(box<T,dim> const& box, size_t axis)
{
    return box.center()[axis] + half_extent(box, axis);
}

template<typename T, int dim>
vector<T,dim> min_position(box<T,dim> const& box)
{
    return box.center() - box.half_axis_lengths();
}

template<typename T, int dim>
vector<T,dim> max_position(box<T,dim> const& box)
{
    // Try to get around of the integer rounding with half extents.
    return min_position(box) + box.size();
}

template<typename T, int dim>
bool encloses(box<T,dim> const& box, vector<T,dim> const& point)
{
    vector<T,dim> min_corner = box.center() - box.half_axis_lengths();
    vector<T,dim> max_corner = min_corner + box.size();

    for (int i = 0; i < dim; ++i)
    {
        if (point[i] < min_corner[i] || point[i] > max_corner[i])
            return false;
    }
    return true;
}

template<typename T, typename U, int dim>
box<T,dim> static_box_cast(box<U,dim> const& x)
{
    return box<T,dim>(
        static_vector_cast<T>(x.center()),
        static_vector_cast<T>(x.size()));
}

} // namespace pt

#endif
