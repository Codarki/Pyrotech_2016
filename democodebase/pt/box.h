#pragma once
#ifndef PT_BOX_H
#define PT_BOX_H

#include "box_fwd.h"
#include "vector.h"

namespace pt {

/// Templated axis aligned bounding box for any dimension and type. Use
/// predefined types for most common ones.
///
/// \code
/// typedef box<float,1> box1f;
/// typedef box<float,2> box2f;
/// typedef box<float,3> box3f;
/// typedef box<int,1> box1i;
/// typedef box<int,2> box2i;
/// typedef box<int,3> box3i;
/// typedef box<double,1> box1d;
/// typedef box<double,2> box2d;
/// typedef box<double,3> box3d;
/// 
/// vector3f center(0, 1, 2);
/// vector3f size(100, 200, 300);
/// box3f bounding_box(center, size);
/// \endcode
///
/// \internal Constructing box with half_size, could make the rounding error
/// to <2 for integral types. Therefore total size is used (with rounding
/// error <1).
///
template<typename T, int dim>
class box
{
public:
    /// Type and value for creating uninitialized box.
    enum uninitialized {uninitialized};

    /// Construct an empty box.
    box();
    /// Construct an uninitialized box.
    box(enum uninitialized);
    /// Creates a box of \a size at \a center.
    box(vector<T,dim> const& center, vector<T,dim> const& size);

    /// Copy constructs box from different type of box.
    template<typename U>
    box(box<U,dim> const& other);

    /// \return Center position of this box.
    vector<T,dim> const& center() const;
    void set_center(vector<T,dim> const& position);

    /// \return Size of this box.
    vector<T,dim> const& size() const;
    void set_size(vector<T,dim> const& size);

    /// \return Half axis lengths of this box. \note For integral types, this
    /// can result in rounding error, returning
    /// 2*half_axis_lengths == size - 1.
    vector<T,dim> half_axis_lengths() const;

private: // data members
	vector<T,dim> m_center;
	vector<T,dim> m_size;
};

// Free functions.

/// Constructs bounding box from \a min and \a max corners.
/// \return Box constructed to enclose \a min and \a max corners.
/// \relatesalso pt::box
template<typename T, int dim>
box<T,dim> box_from_min_max(vector<T,dim> const& min,
    vector<T,dim> const& max);

/// \return Half extent of a \a box at \a axis.
template<typename T, int dim>
T half_extent(box<T,dim> const& box, size_t axis);

/// \return Minimum of \a box at \a axis. \todo axis_min() could be better.
template<typename T, int dim>
T min_axis(box<T,dim> const& box, size_t axis);
/// \return Maximum of \a box at \a axis.
template<typename T, int dim>
T max_axis(box<T,dim> const& box, size_t axis);

/// \return Minimum corner position of \a box.
template<typename T, int dim>
vector<T,dim> min_position(box<T,dim> const& box);
/// \return Maximum corner position of \a box.
template<typename T, int dim>
vector<T,dim> max_position(box<T,dim> const& box);

/// \return \c true if \a box encloses \a point. \relatesalso pt::box
template<typename T, int dim>
bool encloses(box<T,dim> const& box, vector<T,dim> const& point);

/// \return Box \a x converted to type T.
template<typename T, typename U, int dim>
box<T,dim> static_box_cast(box<U,dim> const& x);

} // namespace pt

#include "box.inl"

#endif
