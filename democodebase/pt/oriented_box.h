#ifndef PT_ORIENTED_BOX_H
#define PT_ORIENTED_BOX_H

#include "vector.h"
#include "matrix_operations.h"

namespace pt {

template<typename T, int dim>
class oriented_box
{
public:
    /// Type and value for creating uninitialized box.
    enum uninitialized {uninitialized};

    /// Construct an empty box.
    oriented_box();
    /// Construct an uninitialized box.
    oriented_box(enum uninitialized);

    /// Sets the center of this box to \a position.
    void set_center(vector<T,dim> const& position);
    /// \return Center position of this box.
    vector<T,dim> const& center() const;

    /// Sets the size of this box at axis \a i to \a new_size.
    void set_size(size_t i, vector<T,dim> const& size);
    /// \return Size of this box at axis \a i.
    vector<T,dim> const& size(size_t i) const;

private: // data members
	vector<T,dim> m_center;
	vector<T,dim> m_sizes[dim];
};

/// 2 dimension float oriented box. \relatesalso oriented_box
typedef oriented_box<float,2> oriented_box2f;
/// 3 dimension float oriented box. \relatesalso oriented_box
typedef oriented_box<float,3> oriented_box3f;

/// 2 dimension integer oriented box. \relatesalso oriented_box
typedef oriented_box<int,2> oriented_box2i;
/// 3 dimension integer oriented box. \relatesalso oriented_box
typedef oriented_box<int,3> oriented_box3i;

// Free functions.

/// \return Half extent of a \a box at \a axis.
template<typename T, int dim>
T half_extent(oriented_box<T,dim> const& box, vector<T,dim> const& axis);

/// \return Half extent of a \a box at \a axis.
template<typename T, int dim>
T half_extent(oriented_box<T,dim> const& box, size_t axis);

template<typename T, typename U>
oriented_box<T,2> transform(box<T,2> const& box,
    matrix<U,3,3> const& matrix);

template<typename T, int dim>
box<T,dim> bounding_box(oriented_box<T,dim> const& box);

} // namespace pt

#include "oriented_box.inl"

#endif
