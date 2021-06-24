#ifndef PT_MATRIX_OPERATIONS_INL
#define PT_MATRIX_OPERATIONS_INL

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
inline vector<T,columns> transform(vector<T,columns> const& v,
    matrix<T,rows,columns> const& m)
{
	vector<T,columns> result(vector<T,columns>::uninitialized);

    for (size_t y = 0; y < rows; ++y)
    {
        result[y] = dot(m.row(y), v);

        //for (size_t x = 0; x < columns; ++x)
        //    result[y] += v[x] * m(y,x);
    }

	return result;
}

template<typename T, typename U>
inline vector<T,3> transform_position(vector<T,3> const& v,
    matrix<U,4,4> const& m)
{
    vector<T,3> result(vector<T,3>::uninitialized);
    //result[0] = m(0,0) * v[0] + m(0,1) * v[1] + m(0,2) * v[2] + m(0,3);
    //result[1] = m(1,0) * v[0] + m(1,1) * v[1] + m(1,2) * v[2] + m(1,3);
    //result[2] = m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2] + m(2,3);

    result[0] = m(0,0) * v[0] + m(1,0) * v[1] + m(2,0) * v[2] + m(3,0);
    result[1] = m(0,1) * v[0] + m(1,1) * v[1] + m(2,1) * v[2] + m(3,1);
    result[2] = m(0,2) * v[0] + m(1,2) * v[1] + m(2,2) * v[2] + m(3,2);

    return result;
}

template<typename T, typename U>
inline vector<T, 3> transform_position_w(vector<T, 3> const& v,
    matrix<U, 4, 4> const& m)
{
    vector<T, 3> result(vector<T, 3>::uninitialized);

    result[0] = m(0, 0) * v[0] + m(1, 0) * v[1] + m(2, 0) * v[2] + m(3, 0);
    result[1] = m(0, 1) * v[0] + m(1, 1) * v[1] + m(2, 1) * v[2] + m(3, 1);
    result[2] = m(0, 2) * v[0] + m(1, 2) * v[1] + m(2, 2) * v[2] + m(3, 2);

    float w = m(0, 3) * v[0] + m(1, 3) * v[1] + m(2, 3) * v[2] + m(3, 3);
    if (w == static_cast<T>(0))
        return vector<T, 3>(0, 0, 0);
    return result / w;
}

template<typename T, typename U>
inline vector<T,4> transform_position(vector<T,4> const& v,
    matrix<U,4,4> const& m)
{
    return vector<T,4>(
        dot(m.row(0), v),
        dot(m.row(1), v),
        dot(m.row(2), v),
        dot(m.row(3), v));
}

template<typename T, typename U>
inline vector<T,2> transform_position(vector<T,2> const& v,
    matrix<U,3,3> const& m)
{
    vector<U,2> const tmp = static_vector_cast<U>(v);
    vector<T,2> result(
        static_cast<T>(m(0,0) * tmp[0] + m(1,0) * tmp[1] + m(2,0)),
        static_cast<T>(m(0,1) * tmp[0] + m(1,1) * tmp[1] + m(2,1)));
    return result;
}

template<typename T>
inline vector<T,2> transform_direction(vector<T,2> const& v,
    matrix<T,3,3> const& m)
{
    return vector<T,2>(
        dot(vector<T,2>(m.row(0)), v),
        dot(vector<T,2>(m.row(1)), v));
}

template<typename T>
inline vector<T,3> transform_direction(vector<T,3> const& v,
    matrix<T,4,4> const& m)
{
    vector<T,4> column0 = m.column(0);
    vector<T,4> column1 = m.column(1);
    vector<T,4> column2 = m.column(2);

    vector<T,3> column0_3 = vector<T,3>(column0[0], column0[1], column0[2]);
    vector<T,3> column1_3 = vector<T,3>(column1[0], column1[1], column1[2]);
    vector<T,3> column2_3 = vector<T,3>(column2[0], column2[1], column2[2]);

    return vector<T,3>(
        dot(column0_3, v),
        dot(column1_3, v),
        dot(column2_3, v));

    //vector<T,4> row0 = m.row(0);
    //vector<T,4> row1 = m.row(1);
    //vector<T,4> row2 = m.row(2);

    //vector<T,3> row0_3 = vector<T,3>(row0[0], row0[1], row0[2]);
    //vector<T,3> row1_3 = vector<T,3>(row1[0], row1[1], row1[2]);
    //vector<T,3> row2_3 = vector<T,3>(row2[0], row2[1], row2[2]);

    //return vector<T,3>(
    //    dot(vector<T,3>(row0_3), v),
    //    dot(vector<T,3>(row1_3), v),
    //    dot(vector<T,3>(row2_3), v));
}

template<typename T>
inline vector<T,4> transform_direction(vector<T,4> const& v,
    matrix<T,4,4> const& m)
{
    vector<T,4> result(vector::uninitialized);
    result[0] = m(0,0) * v[0] + m(0,1) * v[1] + m(0,2) * v[2];
    result[1] = m(1,0) * v[0] + m(1,1) * v[1] + m(1,2) * v[2];
    result[2] = m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2];
    result[3] = v[3]; //m(3,0) * v[0] + m(3,1) * v[1] + m(3,2) * v[3];
    return result;
}

template<typename T, typename U>
inline vector<T,2> transform_direction(vector<T,2> const& v,
    matrix<U,3,3> const& m)
{
    vector<U,2> const tmp = static_vector_cast<U>(v);

    vector<U,3> row0(m.row(0));
    vector<U,3> row1(m.row(1));
    vector<U,2> row0_2(row0[0], row0[1]);
    vector<U,2> row1_2(row1[0], row1[1]);

    return vector<T,2>(
        static_cast<T>(dot(row0_2, tmp)),
        static_cast<T>(dot(row1_2, tmp)));
}

template<typename T, typename U>
inline vector<T,3> transform_direction(vector<T,3> const& v,
    matrix<U,4,4> const& m)
{
    vector<U,3> const tmp = static_vector_cast<U>(v);

    return vector<T,3>(
        dot(vector<U,3>(m.row(0)), tmp),
        dot(vector<U,3>(m.row(1)), tmp),
        dot(vector<U,3>(m.row(2)), tmp));
}

template<typename T, typename U>
inline vector<T,4> transform_direction(vector<T,4> const& v,
    matrix<U,4,4> const& m)
{
    //vector<U,4> const tmp = static_vector_cast<U>(v);

    vector<T,4> result(vector::uninitialized);
    result[0] = m(0,0) * v[0] + m(0,1) * v[1] + m(0,2) * v[2];
    result[1] = m(1,0) * v[0] + m(1,1) * v[1] + m(1,2) * v[2];
    result[2] = m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2];
    result[3] = v[3]; //m(3,0) * v[0] + m(3,1) * v[1] + m(3,2) * v[3];
    return result;
}

template<typename T, int rows, int columns>
inline vector<T,columns> translate(vector<T,columns> const& v,
    matrix<T,rows,columns> const& m)
{
	vector<T,columns> result(vector<T,columns>::uninitialized);

    for (size_t i = 0; i < columns; ++i)
        result[i] = v[i] + m(3,i);

    return result;
}

template<typename T>
matrix<T,4,4> rotation_matrix(vector<T,3> const& axis, T angle)
{
    // Orthogonal matrix for rotation around axis by angle

    // left handed coordinate system rotates clockwise around the axis.
    
    matrix<T,4,4> m(matrix<T,4,4>::uninitialized);

    T const& x = axis[0];
    T const& y = axis[1];
    T const& z = axis[2];
    
    T c = 1.0f - std::cos(angle);
    T s = std::sin(angle);

    m(0,0) = 1.0f + c * (x * x - 1.0f);
    m(0,1) = -z * s + c * x * y;
    m(0,2) = y * s + c * x * z;
    m(0,3) = 0.0f;

    m(1,0) = z * s + c * x * y;
    m(1,1) = 1.0f + c * (y * y - 1.0f);
    m(1,2) = -x * s + c * y * z;
    m(1,3) = 0.0f;

    m(2,0) = -y * s + c * x * z;
    m(2,1) = x * s + c * y *z;
    m(2,2) = 1.0f + c * (z * z - 1.0f);
    m(2,3) = 0.0f;

    m.set_row(3, vector<T,4>(0,0,0,1));

    return m;
}

template<typename T>
matrix<T,4,4> rotation_y_matrix(T radians)
{
    matrix<T,4,4> m(matrix<T,4,4>::uninitialized);
    m.set_row(0, vector<T,4>(std::cos(radians), 0, -std::sin(radians), 0));
    m.set_row(1, vector<T,4>(0, 1, 0, 0));
    m.set_row(2, vector<T,4>(std::sin(radians), 0, std::cos(radians), 0));
    m.set_row(3, vector<T,4>(0, 0, 0, 1));
    return m;
}

template<typename T>
matrix<T,3,3> rotation_z_matrix(T radians)
{
    matrix<T,3,3> m(matrix<T,3,3>::uninitialized);
    m.set_row(0, vector<T,3>(std::cos(radians), -std::sin(radians), 0));
    m.set_row(1, vector<T,3>(std::sin(radians), std::cos(radians), 0));
    m.set_row(2, vector<T,3>(0, 0, 1));
    return m;
}

template<typename T>
matrix<T,4,4> translation_matrix(vector<T,3> const& translation)
{
    matrix<T,4,4> m;
    set_translation(m, translation);
    return m;
}

template<typename T>
matrix<T,3,3> translation_matrix(vector<T,2> const& translation)
{
    matrix<T,3,3> m;
    set_translation(m, translation);
    return m;
}

template<typename T>
matrix<T,4,4> scaling_matrix(vector<T,3> const& scale)
{
    matrix<T,4,4> m;
    m(0,0) = scale[0];
    m(1,1) = scale[1];
    m(2,2) = scale[2];
    return m;
}

template<typename T>
matrix<T,3,3> scaling_matrix(vector<T,2> const& scale)
{
    matrix<T,3,3> m;
    m(0,0) = scale[0];
    m(1,1) = scale[1];
    return m;
}

template<typename T>
matrix<T,3,3> scaling_matrix(T x, T y)
{
    return scaling_matrix(vector<T,2>(x, y));
}

template<typename T>
inline void set_translation(matrix<T,4,4>& matrix,
    vector<T,3> const& v)
{
    matrix.set_row(3, vector<T,4>(v, static_cast<T>(1)));
}

template<typename T>
inline void set_translation(matrix<T,3,3>& matrix,
    vector<T,2> const& v)
{
    matrix.set_row(2, vector<T,3>(v, static_cast<T>(1)));
}

template<typename T>
inline vector<T,3> basis_axis_x(matrix<T,4,4> const& m)
{
    vector<T,4> column = m.column(0);
    return vector<T,3>(column[0], column[1], column[2]);
}

template<typename T>
inline vector<T,3> basis_axis_y(matrix<T,4,4> const& m)
{
    vector<T,4> column = m.column(1);
    return vector<T,3>(column[0], column[1], column[2]);
}

template<typename T>
inline vector<T,3> basis_axis_z(matrix<T,4,4> const& m)
{
    vector<T,4> column = m.column(2);
    return vector<T,3>(column[0], column[1], column[2]);
}

template<typename T>
inline vector<T,3> translation(matrix<T,4,4> const& m)
{
    vector<T,4> row = m.row(3);
	return vector<T, 3>(row[0], row[1], row[2]);// / row[3];
}

template<typename T>
inline vector<T,3> basis_axis_lengths(matrix<T,4,4> const& m)
{
    return vector<T,3>(length(basis_axis_x(m)), length(basis_axis_y(m)),
        length(basis_axis_z(m)));
}

template<typename T, int rows, int columns>
matrix<T,columns,rows> transpose(matrix<T,rows,columns> const& m)
{
    matrix<T,columns,rows> result(matrix<T,columns,rows>::uninitialized);

    for (int i = 0; i < columns; ++i)
        result.set_row(i, m.column(i));

    return result;
}

template<typename T>
inline T determinant(matrix<T,2,2> const& m)
{
    return m(0,0) * m(1,1) - m(0,1) * m(1,0);
}

template<typename T>
inline T determinant(matrix<T,3,3> const& m)
{
    // Laplacian expansion by minors.

    // expansion by the first column
    T term1 = m(0,0) * m(1,1) * m(3,3) - m(0,0) * m(1,2) * m(2,1);
    T term2 = m(1,0) * m(0,1) * m(2,2) - m(1,0) * m(0,2) * m(2,1);
    T term3 = m(2,0) * m(0,1) * m(1,2) - m(2,0) * m(0,2) * m(1,1);

    // expansion by the first row
    //T term1 = m(0,0) * m(1,1) * m(2,2) - m(0,0) * m(1,2) * m(2,1);
    //T term2 = m(0,1) * m(1,0) * m(2,2) - m(0,1) * m(1,2) * m(2,0);
    //T term3 = m(0,2) * m(1,0) * m(2,1) - m(0,2) * m(1,1) * m(2,0);

    return term1 - term2 + term3;
}

template<typename T>
T determinant(matrix<T,4,4> const& m)
{
    float a
        = ((m(0,3)*m(1,2) - m(0,2)*m(1,3)) * m(2,1)
        +  (m(0,1)*m(1,3) - m(0,3)*m(1,1)) * m(2,2)
        +  (m(0,2)*m(1,1) - m(0,1)*m(1,2)) * m(2,3)) * m(3,0);

    float b
        = ((m(0,3)*m(1,0) - m(0,0)*m(1,3)) * m(2,2)
        +  (m(0,0)*m(1,2) - m(0,2)*m(1,0)) * m(2,3)
        +  (m(0,2)*m(1,3) - m(0,3)*m(1,2)) * m(2,0)) * m(3,1);

    float c
        = ((m(0,1)*m(1,0) - m(0,0)*m(1,1)) * m(2,3)
        +  (m(0,3)*m(1,1) - m(0,1)*m(1,3)) * m(2,0)
        +  (m(0,0)*m(1,3) - m(0,3)*m(1,0)) * m(2,1)) * m(3,2);

    float d
        = ((m(0,1)*m(1,2) - m(0,2)*m(1,1)) * m(2,0)
        +  (m(0,2)*m(1,0) - m(0,0)*m(1,2)) * m(2,1)
        +  (m(0,0)*m(1,1) - m(0,1)*m(1,0)) * m(2,2)) * m(3,3);

    return a + b + c + d;
}

template<typename T, int rows, int columns>
inline matrix<T,columns,rows> invert(matrix<T,columns,rows> const& m)
{
    throw_matrix_no_general_function_implemented("invert");
    return transpose(m);
}

template<typename T>
inline matrix<T,2,2> invert(matrix<T,2,2> const& m)
{
    T const& a = m(0,0);
    T const& b = m(0,1);
    T const& c = m(1,0);
    T const& d = m(1,1);

    matrix<T,2,2> result;
    result.set_row(0, vector<T,2>(d, -b));
    result.set_row(1, vector<T,2>(-c, a));

    // Calculate matrix inverse.
    T det = static_cast<T>(1) / determinant(m);

    result *= det;

    return result;
}

template<typename T>
inline matrix<T,3,3> invert(matrix<T,3,3> const& m)
{
    // InvM = (1/det(M)) * Transpose(Cofactor(M))

    T determinant = determinant(m);

    T det_00 = determinant(make_matrix2x2(m(1,1), m(1,2), m(2,1), m(2,2)));
    T det_10 = -determinant(make_matrix2x2(m(0,1), m(0,2), m(2,1), m(2,2)));
    T det_20 = determinant(make_matrix2x2(m(0,1), m(0,2), m(1,1), m(1,2)));

    T det_01 = -determinant(make_matrix2x2(m(1,0), m(1,2), m(2,0), m(2,2)));
    T det_11 = determinant(make_matrix2x2(m(0,0), m(0,2), m(2,0), m(2,2)));
    T det_21 = -determinant(make_matrix2x2(m(0,0), m(0,2), m(1,0), m(1,2)));

    T det_02 = determinant(make_matrix2x2(m(1,0), m(1,1), m(2,0), m(2,1)));
    T det_12 = -determinant(make_matrix2x2(m(0,0), m(0,1), m(2,0), m(2,1)));
    T det_22 = determinant(make_matrix2x2(m(0,0), m(0,1), m(1,0), m(1,1)));

    // transpose as we go

    vector<T,3> row1(det_00, det_10, det_20);
    vector<T,3> row2(det_01, det_11, det_21);
    vector<T,3> row3(det_02, det_12, det_22);

    // cofactor matrix is the matrix of determinants of the minors

    matrix4x4f transposed_cofactor_matrix(make_matrix3x3(row1, row2, row3));

    return static_cast<T>(1) / determinant(m)
        * transposed_cofactor_matrix;
}

template<typename T>
matrix<T,4,4> invert(matrix<T,4,4> const& m)
{
    return detail::inversion_with_cramers_rule(m);
}

template<typename T, int rows, int columns>
bool has_orthonormal_basis(matrix<T,columns,rows> const& m)
{
    throw_matrix_no_general_function_implemented("has_orthonormal_basis");
    return false;
}

template<typename T>
bool has_orthonormal_basis(matrix<T,2,2> const& m)
{
    T const column1_length = length(m.column(0));
    T const column2_length = length(m.column(1));
    T const cos1 = dot(m.column(0), m.column(1));
    T const cos2 = dot(m.column(0), m.column(2));
    T const epsilon = static_cast<T>(0.00001);
    T const unit_min = static_cast<T>(1.0) - epsilon;
    T const unit_max = static_cast<T>(1.0) + epsilon;
    T const zero_min = static_cast<T>(0.0) - epsilon;
    T const zero_max = static_cast<T>(0.0) + epsilon;
    
    bool has_unit_columns
        = (column1_length > unit_min && column1_length < unit_max)
        && (column2_length > unit_min && column2_length < unit_max);

    bool is_orthogonal_basis
        = (cos1 > zero_min && cos1 < zero_max)
        && (cos2 > zero_min && cos2 < zero_max);
        
    return has_unit_columns && is_orthogonal_basis;
}

template<typename T>
bool has_orthonormal_basis(matrix<T,3,3> const& m)
{
    T const column1_length = length(m.column(0));
    T const column2_length = length(m.column(1));
    T const column3_length = length(m.column(2));
    
    T const cos1 = dot(m.column(0), m.column(1));
    T const cos2 = dot(m.column(0), m.column(2));
    T const cos3 = dot(m.column(1), m.column(2));
    
    // Lets not mind about precision errors.
    
    T const epsilon = static_cast<T>(0.00001);
    T const unit_min = static_cast<T>(1.0) - epsilon;
    T const unit_max = static_cast<T>(1.0) + epsilon;
    T const zero_min = static_cast<T>(0.0) - epsilon;
    T const zero_max = static_cast<T>(0.0) + epsilon;
    
    bool has_unit_columns
        = (column1_length > unit_min && column1_length < unit_max)
        && (column2_length > unit_min && column2_length < unit_max)
        && (column3_length > unit_min && column3_length < unit_max);

    bool is_orthogonal_basis
        = (cos1 > zero_min && cos1 < zero_max)
        && (cos2 > zero_min && cos2 < zero_max)
        && (cos3 > zero_min && cos3 < zero_max);
        
    return has_unit_columns && is_orthogonal_basis;
}

template<typename T>
bool is_orthogonal(matrix<T,3,3> const& m)
{
    // Matrix is orthogonal only if its columns form an orthonormal basis of
    // the Euclidean space. The determinant of any orthogonal matrix is +1 or
    // -1.
    
    T const abs_determinant = std::abs(determinant(m));
    T const epsilon = static_cast<T>(0.00001);
    
    return (abs_determinant > static_cast<T>(1.0) - epsilon)
        && (abs_determinant < static_cast<T>(1.0) + epsilon)
        && has_orthonormal_basis(m);
}

template<typename T, int rows, int columns>
matrix<T,columns,rows> orthogonalize(matrix<T,columns,rows> const& m)
{
    throw_matrix_no_general_function_implemented("orthogonalize");
    return m;
}

template<typename T>
matrix<T,2,2> orthogonalize(matrix<T,2,2> const& m)
{
    // Stabilized Gram–Schmidt orthogonalize.
    
    matrix<T,2,2> result(matrix::uninitialized);
        
    vector<T,2> const& col1 = m.column(0);
    vector<T,2> const& col2 = m.column(1);
        
    result.set_column(0, normalize(col1));
    result.set_column(1, normalize(col2 - dot(col2, col1) * col1));
    
    return result;
}

template<typename T>
matrix<T,3,3> orthogonalize(matrix<T,3,3> const& m)
{
    // Stabilized Gram–Schmidt orthogonalize.
    
    matrix<T,3,3> result(matrix::uninitialized);
        
    vector<T,3> const& col1 = m.column(0);
    vector<T,3> const& col2 = m.column(1);
    vector<T,3> const& col3 = m.column(2);
        
    result.set_column(0, normalize(col1));
    result.set_column(1, normalize(col2 - dot(col2, col1) * col1));
    result.set_column(2, normalize(col3 - dot(col3, col1) * col1
        - dot(col3, col2) * col2));
    
    return result;
}

template<typename T>
matrix<T,4,4> orthogonalize(matrix<T,4,4> const& m)
{
    // Stabilized Gram–Schmidt orthogonalize.

    matrix<T,4,4> result(matrix<T,4,4>::uninitialized);

    // We leave 4th row untouched.

    vector<T,4> const& row1 = m.row(0);
    vector<T,4> const& row2 = m.row(1);
    vector<T,4> const& row3 = m.row(2);

    result.set_row(0, normalize(row1));
    result.set_row(1, normalize(row2 - dot(row2, row1) * row1));
    result.set_row(2, normalize(row3 - dot(row3, row1) * row1
        - dot(row3, row2) * row2));
    result.set_row(3, m.row(3));

    return result;
}

template<typename T>
inline void orthogonalize_in_place(matrix<T,4,4>& m)
{
    // Stabilized Gram–Schmidt orthogonalize.
    
    // We leave 4th column untouched.
    assert(!"column() does not return by reference!");
    vector<T,4>& col1 = m.column(0);
    vector<T,4>& col2 = m.column(1);
    vector<T,4>& col3 = m.column(2);
        
    col1 = normalize(col1);
    
    // Remove component in col1 direction.
    col2 -= dot(col2, col1) * col1;
    col2 = normalize(col2);
    
    // Remove components in col1 and col2 direction.
    col3 -= dot(col3, col1) * col1;
    col3 -= dot(col3, col2) * col2;
    col3 = normalize(col3);
}

template<typename T, int rows, int columns>
void denormalize_in_place(matrix<T,rows,columns>& m)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
            m(i,j) = denormalize(m(i,j));
    }
}

} // namespace pt

#endif
