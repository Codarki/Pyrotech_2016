#pragma once
#ifndef PT_MATRIX_H
#define PT_MATRIX_H

#include "matrix_fwd.h"
#include "vector.h"

namespace pt {

/// Templated matrix for any dimensions and types. Use predefined type
/// definitions for most common ones. Row-major.
///
/// \code
/// typedef matrix<float,4,4> matrix4x4f;
/// typedef matrix<float,3,3> matrix3x3f;
/// typedef matrix<float,2,2> matrix2x2f;
/// 
/// matrix4x4f my_matrix;
/// my_matrix.row(0) = vector4f(1.0f, 0.0f, 0.0f, 0.0f);
/// \endcode
///
template<typename T, int rows, int columns>
class matrix
{
public: // type definitions
    /// Enum for creating uninitialized matrix.
    enum uninitialized_type
    {
        /// Enum for constructing uninitialized matrix. Without this flag,
        /// matrix is set to identity by default.
        uninitialized
    };

public:
    /// Construct an identity matrix.
    matrix();

    /// Construct an uninitialized matrix.
	matrix(uninitialized_type);

    /// Construct matrix from different element type matrix \a x.
    template<typename U>
	matrix(matrix<U, rows, columns> const& x);

    /// Sets this matrix to identity.
	matrix const& set_identity();

    /// Accessor for rows. \return Row vector.
	vector<T,columns> row(size_t r) const;
    /// Accessor for columns. \return Column vector.
	vector<T,rows> column(size_t c) const;

    /// Sets row vector at \a row_index to \a row.
	void set_row(size_t row_index, vector<T,columns> const& row);
    /// Sets Column vector at \a column_index to \a column.
	void set_column(size_t column_index, vector<T,rows> const& column);

    // [] Accessor for columns. \return vector from column \a c.
    // \note these have ambiguous meaning at best, and thus DEPRACATED.
    // Use column() and set_column() instead.

    /// Accessor for elements. \return Reference to element at
    /// (\a row,\a column).
    T const& operator()(size_t row, size_t column) const;
    /// \overload
    T& operator()(size_t row, size_t column);

    /// Matrix product in-place. \return This matrix multiplied by \a other
    /// matrix.
    template<int q>
	matrix<T,rows,q> const& operator*=(matrix<T,columns,q> const& other);

	//matrix const& operator=(matrix const& x);

    /// Multiplies each element by \a scalar.
	matrix const& operator*=(T const& scalar);

private: // data members
    vector<T, columns> m[rows];
	//T m[rows][columns];
};

// Operators

// TODO move const math operators away from inside the matrix class, and
// implement the missing ones.

/// Matrix product of different dimension matrices. \return Matrix \a a
/// multiplied with matrix \a b. \relatesalso matrix
template<typename T, int rows, int columns, int q>
matrix<T,rows,q> operator*(matrix<T,rows,columns> const& a,
    matrix<T,columns,q> const& b);

/// Matrix product of same dimension matrices. \return Matrix \a a multiplied
/// with matrix \a b. \relatesalso matrix
//template<typename T, int rows, int columns>
//matrix<T,rows,columns> operator*(matrix<T,rows,columns> const& a,
//    matrix<T,rows,columns> const& b);

/// Matrix product with scalar. \return Matrix \a a multiplied with scalar
/// \a b. \relatesalso matrix
template<typename T, int rows, int columns>
matrix<T,rows,columns> operator*(matrix<T,rows,columns> const& a, T const& b);
/// Matrix product with scalar. \return Matrix \a b multiplied with * scalar
/// \a a. \relatesalso matrix
template<typename T, int rows, int columns>
matrix<T,rows,columns> operator*(T const& a, matrix<T,rows,columns> const& b);

/// Matrix addition. \return Matrix \a a added with matrix \a b.
/// \relatesalso matrix.
template<typename T, int rows, int columns>
matrix<T,rows,columns> operator+(matrix<T,rows,columns> const& a,
    matrix<T,rows,columns> const& b);
/// Matrix subtraction. \return Matrix \a a subtracted by matrix \a b.
/// \relatesalso matrix.
template<typename T, int rows, int columns>
matrix<T,rows,columns> operator-(matrix<T,rows,columns> const& a,
    matrix<T,rows,columns> const& b);

template<typename T, int rows, int columns>
matrix<T,rows,columns> operator-(matrix<T,rows,columns> const& a);

} // namespace pt

#include "matrix.inl"
#include "matrix_detail.inl"

#endif
