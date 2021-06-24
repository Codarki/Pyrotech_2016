#include "matrix.h"
#include "matrix_utility.h"
#include <exception>

namespace pt {
namespace detail {
    // These are here, in order to avoid inclusion of <exception> in every
    // file including matrix.h.

    void throw_matrix_not_invertible()
    {
        throw std::exception("matrix not invertible");
    }

    void throw_matrix_no_general_function_implemented(
        char const* function_name)
    {
        std::string error = std::string("No general version for template ")
            + "function: matrix<T,rows,columns>::" + function_name;

            throw std::exception(error.c_str());
    }
} // namespace detail
} // namespace pt
