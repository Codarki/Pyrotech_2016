#ifndef PT_MATRIX_UTILITY_H
#define PT_MATRIX_UTILITY_H

#include "matrix.h"
#include "defs.h"
#include "output_stream.h"
#include "input_stream.h"
#include "vector_utility.h"

namespace pt {

// TODO Pretty sure these doesnt get callled at all. These should be
// specializations of read and write functions from streams. Make em work.
// Same with vectors.

/// Read \a matrix from \a stream. \relatesalso matrix
/// \relatesalso input_stream
template<typename T, int rows, int columns>
inline void read(input_stream& stream, matrix<T,rows,columns>& matrix)
{
    for (int i = 0; i < rows; ++i)
    {
        vector<T,columns> v;
        pt::read<T,columns>(stream, v);

        matrix.set_row(i, v);
    }
}

/// Write \a matrix into \a stream. \relatesalso matrix
/// \relatesalso output_stream
template<typename T, int rows, int columns>
inline void write(output_stream& stream, matrix<T,rows,columns> const& matrix)
{
    for (int i = 0; i < rows; ++i)
    {
        write(stream, matrix.row(i));
    }
}

} // namespace pt

#endif
