#ifndef PT_MATRIX_INL
#define PT_MATRIX_INL

namespace pt {
namespace detail {
    void throw_matrix_not_invertible();
    void throw_matrix_no_general_function_implemented(
        char const* function_name);

    template<typename T>
    matrix<T,2,2> make_matrix2x2(T const& m11, T const& m12, T const& m21,
        T const& m22);

    template<typename T>
    matrix<T,2,2> make_matrix2x2(matrix<T,4,4> const& m, size_t row,
        size_t column);

    template<typename T>
    matrix<T,3,3> make_matrix3x3(vector<T,3> const& row1,
        vector<T,3> const& row2, vector<T,3> const& row3);

    template<typename T>
    matrix<T,4,4> make_matrix4x4(matrix<T,2,2> const& m1,
        matrix<T,2,2> const& m2, matrix<T,2,2> const& m3,
        matrix<T,2,2> const& m4);

    template<typename T>
    matrix<T,4,4> inversion_with_cramers_rule(matrix<T,4,4> const& m);
    template<typename T>
    matrix<T,4,4> inversion_with_cramers_rule2(matrix<T,4,4> const& m);
    template<typename T>
    matrix<T,4,4> invert_by_partitioning(matrix<T,4,4> const& m);

} // namespace detail

template<typename T, int rows, int columns>
inline matrix<T,rows,columns>::matrix()
{
    set_identity();
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns>::matrix(uninitialized_type)
{
}

/*template<typename T, int r, int c>
inline matrix<T,r,c>::matrix(T const* a)
{
    T* tmp = a;
    for (int y = 0; y < c; ++y)
    {
        for (int x = 0; x < r; ++x)
        {
            m[y][x] = *tmp++;
        }
    }
}*/

template<typename T,int rows, int columns>
template<typename U>
inline matrix<T,rows,columns>::matrix(matrix<U,rows,columns> const& x)
{
    for (int i = 0; i < rows; ++i)
        m[i] = x.row(i);
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> const& matrix<T,rows,columns>::set_identity()
{
    size_t tmp = 0;
    for (size_t i = 0; i < rows; ++i)
    {
        vector<T,columns> v;

        if (tmp < columns)
            v[tmp++] = 1.0f;

        set_row(i, v);
    }
    return *this;
}

template<typename T, int rows, int columns>
inline vector<T,columns> matrix<T,rows,columns>::row(size_t row) const
{
    return m[row];
}

template<typename T, int rows, int columns>
inline vector<T,rows> matrix<T,rows,columns>::column(size_t column) const
{
    vector<T,rows> v(vector<T,rows>::uninitialized);

    for (size_t i = 0; i < rows; ++i)
        v[i] = m[i][column];

    return v;
}

template<typename T, int rows, int columns>
inline void matrix<T,rows,columns>::set_row(size_t row_index,
    vector<T,columns> const& row)
{
    m[row_index] = row;
}

template<typename T, int rows, int columns>
inline void matrix<T,rows,columns>::set_column(size_t column_index,
    vector<T,rows> const& column)
{
    for (size_t i = 0; i < rows; ++i)
        m[i][column_index] = column[i];
}

/*template<typename T, int r, int columns>
inline vector<T,r> const& matrix<T,r,columns>::operator[](size_t c) const
{
    return m[c];
}

template<typename T, int r, int columns>
inline vector<T,r>& matrix<T,r,columns>::operator[](size_t c)
{
    return m[c];
}*/

template<typename T, int rows, int columns>
inline T const& matrix<T,rows,columns>::operator()(size_t row_index,
    size_t column_index) const
{
    return m[row_index][column_index];
}

template<typename T, int rows, int columns>
inline T& matrix<T,rows,columns>::operator()(size_t row_index,
    size_t column_index)
{
    return m[row_index][column_index];
}

//template<typename T, int rows, int columns>
//inline matrix<T,rows,columns> const& matrix<T,rows,columns>::operator=(
//    matrix<T,rows,columns> const& x)
//{
//    for (int i = 0; i < rows; ++i)
//        m[i] = x.row(i);
//
//    return *this;
//}

template<typename T, int rows, int columns>
template<int q>
matrix<T,rows,q> const& matrix<T,rows,columns>::operator*=(
    matrix<T,columns,q> const& other)
{
    matrix<T,rows,q> result(matrix<T,rows,q>::uninitialized);

	for (int x = 0; x < q; ++x)
	{
    	for (int y = 0; y < rows; ++y)
            (*this)(y,x) = dot(row(y), other.column(x));
	}

	return *this;
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> const& matrix<T,rows,columns>::operator*=(
    T const& scalar)
{
	for (int i = 0; i < rows; ++i)
        set_row(i, row(i) * scalar);

	return *this;
}

///////////////////

template<typename T, int rows, int columns, int q>
inline matrix<T,rows,q> operator*(matrix<T,rows,columns> const& a,
    matrix<T,columns,q> const& b)
{
    matrix<T,rows,q> result(matrix<T,rows,q>::uninitialized);

	for (int x = 0; x < q; ++x)
	{
    	for (int y = 0; y < rows; ++y)
            result(y,x) = dot(a.row(y), b.column(x));
	}

	return result;
}

//template<typename T, int rows, int columns>
//inline matrix<T,rows,columns> operator*(matrix<T,rows,columns> const& a,
//    matrix<T,rows,columns> const& b)
//{
//    matrix<T,rows,columns> result(matrix<T,rows,columns>::uninitialized);
//
//	for (int x = 0; x < columns; ++x)
//	{
//    	for (int y = 0; y < rows; ++y)
//            result[x][y] = dot(a.row(y), b.column(x));
//	}
//
//    return result;
//}

template<typename T>
inline matrix<T,4,4> operator* (matrix<T,4,4> const& a,
    matrix<T,4,4> const& b)
{
    matrix<T,4,4> result(matrix<T,4,4>::uninitialized);

    for (int i = 0; i < 4; ++i)
    {
        vector<T,4> const row(a.row(i));
        result(i,0) = dot(row, b.column(0));
        result(i,1) = dot(row, b.column(1));
        result(i,2) = dot(row, b.column(2));
        result(i,3) = dot(row, b.column(3));
    }

	return result;
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> operator*(matrix<T,rows,columns> const& a,
    T const& b)
{
    matrix<T,rows,columns> result(a);
    return a *= b;
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> operator*(T const& a,
    matrix<T,rows,columns> const& b)
{
    return b * a;
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> operator+(matrix<T,rows,columns> const& a,
    matrix<T,rows,columns> const& b)
{
    matrix<T,rows,columns> result(matrix<T,rows,columns>::uninitialized);

	for (int i = 0; i < rows; ++i)
        result.set_row(i, a.row(i) + b.row(i));

	return result;
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> operator-(matrix<T,rows,columns> const& a,
    matrix<T,rows,columns> const& b)
{
    matrix<T,rows,columns> result(matrix<T,rows,columns>::uninitialized);

	for (int i = 0; i < rows; ++i)
        result.set_row(i, a.row(i) - b.row(i));

	return result;
}

template<typename T, int rows, int columns>
inline matrix<T,rows,columns> operator-(matrix<T,rows,columns> const& a)
{
    matrix<T,rows,columns> result(matrix<T,rows,columns>::uninitialized);

	for (int i = 0; i < rows; ++i)
        result.set_row(i, -a.row(i));

	return result;
}

} // namespace pt

#endif
