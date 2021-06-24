#pragma once
#ifndef PT_MATRIX_FWD_H
#define PT_MATRIX_FWD_H

namespace pt {

// Matrix forward declaration and typedefs.

template<typename T, int rows, int columns>
class matrix;

typedef matrix<float,4,4> matrix4x4f; //!< 4x4 float matrix \relatesalso matrix
typedef matrix<float,3,3> matrix3x3f; //!< 3x3 float matrix \relatesalso matrix
typedef matrix<float,2,2> matrix2x2f; //!< 2x2 float matrix \relatesalso matrix

} // namespace pt

#endif
