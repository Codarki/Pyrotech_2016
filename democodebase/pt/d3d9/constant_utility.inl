#ifndef PT_D3D9_CONSTANT_UTILITY_INL
#define PT_D3D9_CONSTANT_UTILITY_INL

namespace pt { namespace d3d9 {

namespace detail {
    void check_is_scalar(constant const& constant);
    void check_is_vector(constant const& constant);
    void check_is_matrix(constant const& constant);
    void check_component_type(constant const& constant, float value);
    void check_component_type(constant const& constant, int value);
	void check_component_type(constant const& constant, bool value);
	
    void set_constant(constant_buffer const& buffer,
        constant const& constant, float value);
    void set_vector_constant(constant_buffer const& buffer,
        constant const& constant, vector4f const& value);
    void set_matrix_constant(constant_buffer const& buffer,
        constant const& constant, matrix4x4f const& value);
    void set_vector_array_constant(constant_buffer const& buffer,
        constant const& constant, std::vector<vector4f> const& value);
    void set_matrix_array_constant(constant_buffer const& buffer,
        constant const& constant, matrix4x4f const values[], size_t count);
} // namespace detail

template<typename T>
inline void set_constant(constant const& constant,
    constant_buffer const& buffer, T const& value)
{
    detail::check_is_scalar(constant);
    detail::check_component_type(constant, value);
    detail::set_constant(buffer, constant, value);
}

template<typename T, int dim>
inline void set_constant(constant const& constant,
    constant_buffer const& buffer, vector<T,dim> const& value)
{
    detail::check_is_vector(constant);
    detail::check_component_type(constant, value[0]);
    detail::set_vector_constant(buffer, constant, value);
}

template<typename T, int rows, int columns>
inline void set_constant(constant const& constant,
    constant_buffer const& buffer, matrix<T,rows,columns> const& value)
{
    detail::check_is_matrix(constant);
    detail::check_component_type(constant, value(0, 0));
    detail::set_matrix_constant(buffer, constant, value);
}

template<typename T, int dim>
inline void set_constant(constant const& constant,
    constant_buffer const& buffer, std::vector<vector<T,dim> > const& values)
{
    assert(constant.is_array());
    assert(constant.element_count() == values.size());

    detail::set_vector_array_constant(buffer, constant, values);
}

template<typename T>
void set_constant(constant const& constant, constant_buffer const& buffer,
    T const values[], size_t count);
//{
//    assert(count > 0);
//    assert(constant.is_array());
//    detail::check_component_type(constant, values[0]);
//    detail::set_vector_constant(buffer, constant, values, count);
//}

template<>
inline void set_constant(constant const& constant,
    constant_buffer const& buffer, matrix4x4f const values[], size_t count)
{
    assert(count > 0);
    assert(constant.is_array());

    detail::set_matrix_array_constant(buffer, constant, values, count);
}


}} // namespace pt::d3d9

#endif
