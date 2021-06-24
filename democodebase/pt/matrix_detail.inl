#ifndef PT_MATRIX_DETAIL_INL
#define PT_MATRIX_DETAIL_INL

// these specializations are hidden here, declared in matrix.inl.

namespace pt { namespace detail {

template<typename T>
matrix<T,2,2> make_matrix2x2(T const& m11, T const& m12, T const& m21,
    T const& m22)
{
    matrix<T,2,2> m(matrix::uninitialized);
    m(0,0) = m11;
    m(0,1) = m12;
    m(1,0) = m21;
    m(1,1) = m22;
    return m;
}

template<typename T>
matrix<T,2,2> make_matrix2x2(matrix<T,4,4> const& m, size_t row,
    size_t column)
{
    matrix<T,2,2> result(matrix<T,2,2>::uninitialized);
    result(0,0) = m(row + 0, column + 0);
    result(0,1) = m(row + 0, column + 1);
    result(1,0) = m(row + 1, column + 0);
    result(1,1) = m(row + 1, column + 1);
    return result;
}

template<typename T>
matrix<T,3,3> make_matrix3x3(vector<T,3> const& row1,
    vector<T,3> const& row2, vector<T,3> const& row3)
{
    matrix<T,3,3> m(matrix<T,3,3>::uninitialized);
    m.set_row(0, row1);
    m.set_row(1, row2);
    m.set_row(2, row3);
    return m;
}

template<typename T>
matrix<T,4,4> make_matrix4x4(matrix<T,2,2> const& m1,
    matrix<T,2,2> const& m2, matrix<T,2,2> const& m3,
    matrix<T,2,2> const& m4)
{
    matrix<T,4,4> m(matrix<T,4,4>::uninitialized);
    m.set_row(0, vector<T,4>(m1(0,0), m1(0,1), m2(0,0), m2(0,1)));
    m.set_row(1, vector<T,4>(m1(1,0), m1(1,1), m2(1,0), m2(1,1)));
    m.set_row(2, vector<T,4>(m3(0,0), m3(0,1), m4(0,0), m4(0,1)));
    m.set_row(3, vector<T,4>(m3(1,0), m3(1,1), m4(1,0), m4(1,1)));
    return m;
}

template<typename T>
matrix<T,4,4> inversion_with_cramers_rule(matrix<T,4,4> const& m)
{
    // Taken from: 24504301.pdf
    // Intel: AP-928 Streaming SIMD Extensions -Inverse of 4x4 Matrix
    // 4x4-matrix inversion with Cramer's Rule (without Streaming SIMD
    // Extensions)

    matrix<T,4,4> src = transpose(m);
    matrix<T,4,4> dst;

    // temp array for pair values
    T tmp[12];

    // Calculate pairs for first 8 elements (cofactors).
    tmp[0] = src(2,2) * src(3,3);
    tmp[1] = src(2,3) * src(3,2);
    tmp[2] = src(2,1) * src(3,3);
    tmp[3] = src(2,3) * src(3,1);
    tmp[4] = src(2,1) * src(3,2);
    tmp[5] = src(2,2) * src(3,1);
    tmp[6] = src(2,0) * src(3,3);
    tmp[7] = src(2,3) * src(3,0);
    tmp[8] = src(2,0) * src(3,2);
    tmp[9] = src(2,2) * src(3,0);
    tmp[10] = src(2,0) * src(3,1);
    tmp[11] = src(2,1) * src(3,0);

    // calculate first 8 elements (cofactors)
    dst(0,0) = tmp[0]*src(1,1) + tmp[3]*src(1,2) + tmp[4]*src(1,3);
    dst(0,0) -= tmp[1]*src(1,1) + tmp[2]*src(1,2) + tmp[5]*src(1,3);
    dst(0,1) = tmp[1]*src(1,0) + tmp[6]*src(1,2) + tmp[9]*src(1,3);
    dst(0,1) -= tmp[0]*src(1,0) + tmp[7]*src(1,2) + tmp[8]*src(1,3);
    dst(0,2) = tmp[2]*src(1,0) + tmp[7]*src(1,1) + tmp[10]*src(1,3);
    dst(0,2) -= tmp[3]*src(1,0) + tmp[6]*src(1,1) + tmp[11]*src(1,3);
    dst(0,3) = tmp[5]*src(1,0) + tmp[8]*src(1,1) + tmp[11]*src(1,2);
    dst(0,3) -= tmp[4]*src(1,0) + tmp[9]*src(1,1) + tmp[10]*src(1,2);
    dst(1,0) = tmp[1]*src(0,1) + tmp[2]*src(0,2) + tmp[5]*src(0,3);
    dst(1,0) -= tmp[0]*src(0,1) + tmp[3]*src(0,2) + tmp[4]*src(0,3);
    dst(1,1) = tmp[0]*src(0,0) + tmp[7]*src(0,2) + tmp[8]*src(0,3);
    dst(1,1) -= tmp[1]*src(0,0) + tmp[6]*src(0,2) + tmp[9]*src(0,3);
    dst(1,2) = tmp[3]*src(0,0) + tmp[6]*src(0,1) + tmp[11]*src(0,3);
    dst(1,2) -= tmp[2]*src(0,0) + tmp[7]*src(0,1) + tmp[10]*src(0,3);
    dst(1,3) = tmp[4]*src(0,0) + tmp[9]*src(0,1) + tmp[10]*src(0,2);
    dst(1,3) -= tmp[5]*src(0,0) + tmp[8]*src(0,1) + tmp[11]*src(0,2);

    // calculate pairs for second 8 elements (cofactors)
    tmp[0] = src(0,2)*src(1,3);
    tmp[1] = src(0,3)*src(1,2);
    tmp[2] = src(0,1)*src(1,3);
    tmp[3] = src(0,3)*src(1,1);
    tmp[4] = src(0,1)*src(1,2);
    tmp[5] = src(0,2)*src(1,1);
    tmp[6] = src(0,0)*src(1,3);
    tmp[7] = src(0,3)*src(1,0);
    tmp[8] = src(0,0)*src(1,2);
    tmp[9] = src(0,2)*src(1,0);
    tmp[10] = src(0,0)*src(1,1);
    tmp[11] = src(0,1)*src(1,0);

    // calculate second 8 elements (cofactors)
    dst(2,0) = tmp[0]*src(3,1) + tmp[3]*src(3,2) + tmp[4]*src(3,3);
    dst(2,0) -= tmp[1]*src(3,1) + tmp[2]*src(3,2) + tmp[5]*src(3,3);
    dst(2,1) = tmp[1]*src(3,0) + tmp[6]*src(3,2) + tmp[9]*src(3,3);
    dst(2,1) -= tmp[0]*src(3,0) + tmp[7]*src(3,2) + tmp[8]*src(3,3);
    dst(2,2) = tmp[2]*src(3,0) + tmp[7]*src(3,1) + tmp[10]*src(3,3);
    dst(2,2)-= tmp[3]*src(3,0) + tmp[6]*src(3,1) + tmp[11]*src(3,3);
    dst(2,3) = tmp[5]*src(3,0) + tmp[8]*src(3,1) + tmp[11]*src(3,2);
    dst(2,3)-= tmp[4]*src(3,0) + tmp[9]*src(3,1) + tmp[10]*src(3,2);
    dst(3,0) = tmp[2]*src(2,2) + tmp[5]*src(2,3) + tmp[1]*src(2,1);
    dst(3,0)-= tmp[4]*src(2,3) + tmp[0]*src(2,1) + tmp[3]*src(2,2);
    dst(3,1) = tmp[8]*src(2,3) + tmp[0]*src(2,0) + tmp[7]*src(2,2);
    dst(3,1)-= tmp[6]*src(2,2) + tmp[9]*src(2,3) + tmp[1]*src(2,0);
    dst(3,2) = tmp[6]*src(2,1) + tmp[11]*src(2,3) + tmp[3]*src(2,0);
    dst(3,2)-= tmp[10]*src(2,3) + tmp[2]*src(2,0) + tmp[7]*src(2,1);
    dst(3,3) = tmp[10]*src(2,2) + tmp[4]*src(2,0) + tmp[9]*src(2,1);
    dst(3,3)-= tmp[8]*src(2,1) + tmp[11]*src(2,2) + tmp[5]*src(2,0);

    // Calculate determinant

    T determinant = src(0,0) * dst(0,0) + src(0,1) * dst(0,1)
        + src(0,2) * dst(0,2) + src(0,3) * dst(0,3);

    // Calculate matrix inverse
    determinant = static_cast<T>(1) / determinant;

    dst *= determinant;
    return dst;
}

template<typename T>
matrix<T,4,4> inversion_with_cramers_rule2(matrix<T,4,4> const& m)
{
    // Reordered version of above, reduced 48 scalar multiplications.

    matrix4x4f src = transpose(m);
    matrix4x4f dst;

    // temp array for pair values
    T tmp[12];

/*   0  1  2  3
     4  5  6  7
     8  9 10 11
    12 13 14 15 */

    // Calculate pairs for first 8 elements (cofactors).
    tmp[0] = src(2,2) * src(3,3);
    tmp[1] = src(2,3) * src(3,2);
    tmp[2] = src(2,1) * src(3,3);
    tmp[3] = src(2,3) * src(3,1);
    tmp[4] = src(2,1) * src(3,2);
    tmp[5] = src(2,2) * src(3,1);
    tmp[6] = src(2,0) * src(3,3);
    tmp[7] = src(2,3) * src(3,0);
    tmp[8] = src(2,0) * src(3,2);
    tmp[9] = src(2,2) * src(3,0);
    tmp[10] = src(2,0) * src(3,1);
    tmp[11] = src(2,1) * src(3,0);

    // Calculate first 8 elements (cofactors).
    dst(0,0) = src(1,1) * (tmp[0] - tmp[1]) + src(1,2) * (tmp[3] - tmp[2]) + src(1,3) * (tmp[4] - tmp[5]);
    dst(0,1) = src(1,0) * (tmp[1] - tmp[0]) + src(1,2) * (tmp[6] - tmp[7]) + src(1,3) * (tmp[9] - tmp[8]);
    dst(0,2) = src(1,0) * (tmp[2] - tmp[3]) + src(1,1) * (tmp[7] - tmp[6]) + src(1,3) * (tmp[10] - tmp[11]);
    dst(0,3) = src(1,0) * (tmp[5] - tmp[4]) + src(1,1) * (tmp[8] - tmp[9]) + src(1,2) * (tmp[11] - tmp[10]);
    dst(1,0) = src(0,1) * (tmp[1] - tmp[0]) + src(0,2) * (tmp[2] - tmp[3]) + src(0,3) * (tmp[5] - tmp[4]);
    dst(1,1) = src(0,0) * (tmp[0] - tmp[1]) + src(0,2) * (tmp[7] - tmp[6]) + src(0,3) * (tmp[8] - tmp[9]);
    dst(1,2) = src(0,0) * (tmp[3] - tmp[2]) + src(0,1) * (tmp[6] - tmp[7]) + src(0,3) * (tmp[11] - tmp[10]);
    dst(1,3) = src(0,0) * (tmp[4] - tmp[5]) + src(0,1) * (tmp[9] - tmp[8]) + src(0,2) * (tmp[10] - tmp[11]);

    // Calculate pairs for second 8 elements (cofactors).
    tmp[0] = src(0,2) * src(1,3);
    tmp[1] = src(0,3) * src(1,2);
    tmp[2] = src(0,1) * src(1,3);
    tmp[3] = src(0,3) * src(1,1);
    tmp[4] = src(0,1) * src(1,2);
    tmp[5] = src(0,2) * src(1,1);
    tmp[6] = src(0,0) * src(1,3);
    tmp[7] = src(0,3) * src(1,0);
    tmp[8] = src(0,0) * src(1,2);
    tmp[9] = src(0,2) * src(1,0);
    tmp[10] = src(0,0) * src(1,1);
    tmp[11] = src(0,1) * src(1,0);

    // Calculate second 8 elements (cofactors).
    dst(2,0) = src(3,1) * (tmp[0] - tmp[1]) + src(3,2) * (tmp[3] - tmp[2]) + src(3,3) * (tmp[4] - tmp[5]);
    dst(2,1) = src(3,0) * (tmp[1] - tmp[0]) + src(3,2) * (tmp[6] - tmp[7]) + src(3,3) * (tmp[9] - tmp[8]);
    dst(2,2) = src(3,0) * (tmp[2] - tmp[3]) + src(3,1) * (tmp[7] - tmp[6]) + src(3,3) * (tmp[10] - tmp[11]);
    dst(2,3) = src(3,0) * (tmp[5] - tmp[4]) + src(3,1) * (tmp[8] - tmp[9]) + src(3,2) * (tmp[11] - tmp[10]);
    dst(3,0) = src(2,1) * (tmp[1] - tmp[0]) + src(2,2) * (tmp[2] - tmp[3]) + src(2,3) * (tmp[5] - tmp[4]);
    dst(3,1) = src(2,0) * (tmp[0] - tmp[1]) + src(2,2) * (tmp[7] - tmp[6]) + src(2,3) * (tmp[8] - tmp[9]);
    dst(3,2) = src(2,0) * (tmp[3] - tmp[2]) + src(2,1) * (tmp[6] - tmp[7]) + src(2,3) * (tmp[11] - tmp[10]);
    dst(3,3) = src(2,0) * (tmp[4] - tmp[5]) + src(2,1) * (tmp[9] - tmp[8]) + src(2,2) * (tmp[10] - tmp[11]);

    // Calculate determinant.

    T determinant = src(0,0) * dst(0,0) + src(0,1) * dst(0,1)
        + src(0,2) * dst(0,2) + src(0,3) * dst(0,3);

    // Calculate matrix inverse.
    determinant = static_cast<T>(1) / determinant;

    dst *= determinant;
    return dst;
}

// Inversion by Partitioning: To inverse a matrix A (size N) by partitioning,
// the matrix is partitioned into:
//
//       |  A0    A1  |
//   A = |            | with A0 and A3 squared matrix with the respective size
//       |  A2    A3  |                s0 and s3 following the rule: s0 + s3 = N
//
//The inverse is
//
//          |  B0    B1  |
//   InvA = |            |
//          |  B2    B3  |
//
//with:
//
//  B0 = Inv(A0 - A1 * InvA3 * A2)
//  B1 = - B0 * (A1 * InvA3)
//  B2 = - (InvA3 * A2) * B0
//  B3 = InvA3 + B2 * (A1 * InvA3)

template<typename T>
matrix<T,4,4> invert_by_partitioning(matrix<T,4,4> const& m)
{
    // TODO Tested once, funny results. Recheck.
    matrix<T,2,2> A0(make_matrix2x2(m, 0, 0));
    matrix<T,2,2> A1(make_matrix2x2(m, 0, 2));
    matrix<T,2,2> A2(make_matrix2x2(m, 2, 0));
    matrix<T,2,2> A3(make_matrix2x2(m, 2, 2));

    matrix<T,2,2> inv_A3 = invert(A3);

    matrix<T,2,2> B0(invert(A0 - A1 * inv_A3 * A2));
    matrix<T,2,2> B1(-B0 * (A1 * inv_A3));
    matrix<T,2,2> B2(-(inv_A3 * A2) * B0);
    matrix<T,2,2> B3(inv_A3 + B2 * (A1 * inv_A3));

    return make_matrix4x4(B0, B1, B2, B3);
}

}} // namespace pt::detail

#endif
