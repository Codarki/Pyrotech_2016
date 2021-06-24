#pragma once
#ifndef PT_BOX_FWD_H
#define PT_BOX_FWD_H

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
class box;

/// 1 dimension float box. \relatesalso box
typedef box<float,1> box1f;
/// 2 dimension float box. \relatesalso box
typedef box<float,2> box2f;
/// 3 dimension float box. \relatesalso box
typedef box<float,3> box3f;

/// 1 dimension int box. \relatesalso box
typedef box<int,1> box1i;
/// 2 dimension int box. \relatesalso box
typedef box<int,2> box2i;
/// 3 dimension int box. \relatesalso box
typedef box<int,3> box3i;

/// 1 dimension double box. \relatesalso box
typedef box<double,1> box1d;
/// 2 dimension double box. \relatesalso box
typedef box<double,2> box2d;
/// 3 dimension double box. \relatesalso box
typedef box<double,3> box3d;

} // namespace pt

#endif
