#pragma once
#ifndef PYROTECH_MATRIX_OPERATIONS_H
#define PYROTECH_MATRIX_OPERATIONS_H

#include "math_utils.h"
#include "matrix.h"

namespace pt {

/// Vector transformation. \return Vector \a v transformed by matrix \a m.
/// \relatesalso matrix \relates also vector
template<typename T, int rows, int columns>
vector<T, columns> transform(vector<T,columns> const& v,
    matrix<T,rows,columns> const& m);

/// Vector position transformation. \return Vector \a v transformed by matrix
/// \a m. \relatesalso matrix \relates also vector
template<typename T, typename U>
vector<T,3> transform_position(vector<T,3> const& v, matrix<U,4,4> const& m);
/// Vector position transformation. \return Vector \a v transformed by matrix
/// \a m. \relatesalso matrix \relates also vector
template<typename T, typename U>
vector<T,4> transform_position(vector<T,4> const& v, matrix<U,4,4> const& m);
/// Vector position transformation. \return Vector \a v transformed by matrix
/// \a m. \relatesalso matrix \relates also vector
template<typename T, typename U>
vector<T,2> transform_position(vector<T,2> const& v, matrix<U,3,3> const& m);

/// Vector direction transformation. \return Vector \a v transformed by matrix 
/// \a m, without translation. \relatesalso matrix \relates also vector
template<typename T>
vector<T,2> transform_direction(vector<T,2> const& v, matrix<T,3,3> const& m);
/// Vector direction transformation. \return Vector \a v transformed by matrix 
/// \a m, without translation. \relatesalso matrix \relates also vector
template<typename T>
vector<T,3> transform_direction(vector<T,3> const& v, matrix<T,4,4> const& m);
/// Vector direction transformation. \return Vector \a v transformed by matrix 
/// \a m, without translation. \relatesalso matrix \relates also vector
template<typename T>
vector<T,4> transform_direction(vector<T,4> const& v, matrix<T,4,4> const& m);

/// Vector direction transformation. \return Vector \a v transformed by matrix 
/// \a m, without translation. \relatesalso matrix \relates also vector
template<typename T, typename U>
vector<T,2> transform_direction(vector<T,2> const& v, matrix<U,3,3> const& m);
/// Vector direction transformation. \return Vector \a v transformed by matrix 
/// \a m, without translation. \relatesalso matrix \relates also vector
template<typename T, typename U>
vector<T,3> transform_direction(vector<T,3> const& v, matrix<U,4,4> const& m);
/// Vector direction transformation. \return Vector \a v transformed by matrix
/// \a m, without translation. \relatesalso matrix \relates also vector
template<typename T, typename U>
vector<T,4> transform_direction(vector<T,4> const& v, matrix<U,4,4> const& m);

/// Vector translation. \return Vector \a v translated by matrix \a m.
/// \relatesalso matrix \relates also vector
template<typename T, int rows, int columns>
vector<T, columns> translate(vector<T,columns> const& v,
    matrix<T,rows,columns> const& m);

/// Rotation matrix. \return Rotation matrix around \a axis by \a angle
/// in radians. \relatesalso matrix
template<typename T>
matrix<T,4,4> rotation_matrix(vector<T,3> const& axis, T angle);
/// y-rotation matrix. \return Rotation matrix around y by \a radians.
/// \relatesalso matrix
template<typename T>
matrix<T,4,4> rotation_y_matrix(T radians);
/// 2D z-rotation matrix. \return 2D Rotation matrix around z by \a radians.
/// \relatesalso matrix
template<typename T>
matrix<T,3,3> rotation_z_matrix(T radians);

/// 3D Translation matrix by \a translation. \return 3D Translation matrix.
/// \relatesalso matrix
template<typename T>
matrix<T,4,4> translation_matrix(vector<T,3> const& translation);
/// 2D Translation matrix by \a translation. \return 2D Translation matrix.
/// \relatesalso matrix
template<typename T>
matrix<T,3,3> translation_matrix(vector<T,2> const& translation);

/// 3D Scaling matrix by \a scale. \return 3D Scaling matrix.
/// \relatesalso matrix
template<typename T>
matrix<T,4,4> scaling_matrix(vector<T,3> const& scale);
/// 2D Scaling matrix by \a scale. \return 2DScaling matrix.
/// \relatesalso matrix
template<typename T>
matrix<T,3,3> scaling_matrix(vector<T,2> const& scale);
/// \overload 2D Scaling matrix by \a x and \a y.
template<typename T>
matrix<T,3,3> scaling_matrix(T x, T y);

/// Sets \a matrix's \a translation. \relatesalso matrix
template<typename T>
void set_translation(matrix<T,4,4>& matrix, vector<T,3> const& translation);
/// Sets \a matrix's \a translation. \relatesalso matrix
template<typename T>
void set_translation(matrix<T,3,3>& matrix, vector<T,2> const& translation);

/// Basis axis getter. \return Basis axis x of matrix \a m. \relatesalso matrix
template<typename T>
vector<T,3> basis_axis_x(matrix<T,4,4> const& m);
/// Basis axis getter. \return Basis axis y of matrix \a m. \relatesalso matrix
template<typename T>
vector<T,3> basis_axis_y(matrix<T,4,4> const& m);
/// Basis axis getter. \return Basis axis z of matrix \a m. \relatesalso matrix
template<typename T>
vector<T,3> basis_axis_z(matrix<T,4,4> const& m);
/// Basis axis getter. \return Translation of matrix \a m. \relatesalso matrix
template<typename T>
vector<T,3> translation(matrix<T,4,4> const& m);

/// \return Lengths of basis axises of \a m. \relatesalso matrix
template<typename T>
vector<T,3> basis_axis_lengths(matrix<T,4,4> const& m);

/// \return Transpose of matrix \a m. \relatesalso matrix
template<typename T, int rows, int columns>
matrix<T,columns,rows> transpose(matrix<T,rows,columns> const& m);

// Some misc functions

/// \return Determinant of matrix \a m. \relatesalso matrix
template<typename T, int rows, int columns>
T determinant(matrix<T,rows,columns> const& m);
/// \return Determinant of matrix \a m. \relatesalso matrix
template<typename T>
T determinant(matrix<T,2,2> const& m);
/// \return Determinant of matrix \a m. \relatesalso matrix
template<typename T>
T determinant(matrix<T,3,3> const& m);
/// \return Determinant of matrix \a m. \relatesalso matrix
template<typename T>
T determinant(matrix<T,4,4> const& m);

/// \return Inverse of matrix \a m. \relatesalso matrix
template<typename T, int rows, int columns>
matrix<T,rows,columns> invert(matrix<T,rows,columns> const& m);
/// \return Inverse of matrix \a m. \relatesalso matrix
template<typename T>
matrix<T,2,2> invert(matrix<T,2,2> const& m);
/// \return Inverse of matrix \a m. \relatesalso matrix
template<typename T>
matrix<T,3,3> invert(matrix<T,3,3> const& m);
/// \return Inverse of matrix \a m. \relatesalso matrix
template<typename T>
matrix<T,4,4> invert(matrix<T,4,4> const& m);

/// Checks if matrix \a m has orthonormal basis.
/// Matrix has orthonormal basis only if column vectors are orthogonal and
/// unit length. \return \c true if matrix \a m has orthonormal basis.
/// \relatesalso matrix
template<typename T, int rows, int columns>
bool has_orthonormal_basis(matrix<T,rows,columns> const& m);
/// \overload
/// \relatesalso matrix
template<typename T>
bool has_orthonormal_basis(matrix<T,2,2> const& m);
/// \overload
/// \relatesalso matrix
template<typename T>
bool has_orthonormal_basis(matrix<T,3,3> const& m);
/// \overload
/// \relatesalso matrix
template<typename T>
bool has_orthonormal_basis(matrix<T,4,4> const& m);

/// Checks if matrix \a m is orthogonal. Orthogonal matrix is a square matrix Q
/// whose transpose is its inverse: transpose(Q)*Q = Q*transpose(Q) = identity
/// \return \c true if matrix \a m is orthogonal. \relatesalso matrix
template<typename T>
bool is_orthogonal(matrix<T,3,3> const& m);

/// \return Orthogonalized matrix of \a m. \relatesalso matrix
template<typename T, int rows, int columns>
matrix<T,rows,columns> orthogonalize(matrix<T,rows,columns> const& m);
/// \overload
/// \relatesalso matrix
template<typename T>
matrix<T,4,4> orthogonalize(matrix<T,2,2> const& m);
/// \overload
/// \relatesalso matrix
template<typename T>
matrix<T,4,4> orthogonalize(matrix<T,3,3> const& m);
/// \overload
/// \relatesalso matrix
template<typename T>
matrix<T,4,4> orthogonalize(matrix<T,4,4> const& m);

/// Orthogonalizes matrix \a m.
template<typename T>
void orthogonalize_in_place(matrix<T,4,4>& m);

/// Sets elements near -1, 0 or 1 values to -1, 0 or 1. This helps with
/// precision errors. \relatesalso matrix
template<typename T, int rows, int columns>
void denormalize_in_place(matrix<T,rows,columns>& m);

} // namespace pt

#include "matrix_operations.inl"

#endif
