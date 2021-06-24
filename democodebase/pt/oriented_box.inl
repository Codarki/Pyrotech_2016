#ifndef PT_ORIENTED_BOX_INL
#define PT_ORIENTED_BOX_INL

namespace pt {

template<typename T, int dim>
inline oriented_box<T,dim>::oriented_box()
{
}

template<typename T, int dim>
inline oriented_box<T,dim>::oriented_box(enum uninitialized)
{
}

template<typename T, int dim>
inline void oriented_box<T,dim>::set_center(vector<T,dim> const& position)
{
    m_center = position;
}

template<typename T, int dim>
inline vector<T,dim> const& oriented_box<T,dim>::center() const
{
    return m_center;
}

template<typename T, int dim>
inline void oriented_box<T,dim>::set_size(size_t i,
    vector<T,dim> const& size)
{
    assert(i < dim);
    m_sizes[i] = size;
}

template<typename T, int dim>
inline vector<T,dim> const& oriented_box<T,dim>::size(size_t i) const
{
    assert(i < dim);
    return m_sizes[i];
}

// Free functions

template<typename T, int dim>
T half_extent(oriented_box<T,dim> const& box, vector<T,dim> const& axis)
{
    assert(!"todo");
    T result = static_cast<T>(0);

    for (size_t i = 0; i < dim; ++i)
    {
        //result += dot(box.size(i), axis) * box.size(i);
        result += project(axis, box.size(i));
    }

    return result / 2;
}

template<typename T, int dim>
T half_extent(oriented_box<T,dim> const& box, size_t axis)
{
    assert(!"todo");
    vector<T,dim> const& size = box.size(axis);

    for (size_t i = 0; i < dim; ++i)
    {
    }
}

template<typename T, typename U>
oriented_box<T,2> transform(box<T,2> const& box,
    matrix<U,3,3> const& matrix)
{
    oriented_box<T,2> result;

    for (size_t i = 0; i < 2; ++i)
    {
        vector<T,2> size;
        size[i] = box.size()[i];

        size = transform_direction(size, matrix);
        result.set_size(i, size);
    }

    vector<T,2> center = transform_position(box.center(), matrix);
    result.set_center(center);

    return result;
}

template<typename T, int dim>
box<T,dim> bounding_box(oriented_box<T,dim> const& box)
{
    box<T,dim> result(uninitialized);

    for (size_t i = 0; i < dim; ++i)
    {
    }
}

} // namespace pt

#endif
