#ifndef PT_D3D9_RENDERING_SOURCE_VERTEX_ATTRIBUTE_INL
#define PT_D3D9_RENDERING_SOURCE_VERTEX_ATTRIBUTE_INL

namespace pt { namespace d3d9 { namespace rendering { namespace source {

// vectorf_attribute<dim>

template<int dim>
inline vectorf_attribute<dim>::vectorf_attribute(size_t count)
{
    m_values.resize(count);
}

template<int dim>
inline vectorf_attribute<dim>::vectorf_attribute(
    std::vector<vector_type> const& values)
{
    m_values = values;
}

template<int dim>
inline void vectorf_attribute<dim>::set_at(size_t index,
    vector<float,dim> const& value)
{
    assert(index < m_values.size());
    m_values[index] = value;
}

template<int dim>
inline vertex_element_type vectorf_attribute<dim>::element_type() const
{
    return element_float;
}

template<int dim>
inline size_t vectorf_attribute<dim>::element_count() const
{
    return dim;
}

template<int dim>
inline size_t vectorf_attribute<dim>::count() const
{
    return m_values.size();
}

template<int dim>
inline vector4d vectorf_attribute<dim>::at(size_t index) const
{
    assert(index < m_values.size());
    vector<double,dim> double_value
        = static_vector_cast<double>(m_values[index]);

    return vector4d(double_value);
}

// ubyte4_attribute

inline ubyte4_attribute::ubyte4_attribute(size_t count)
{
    m_values.resize(count);
}

inline ubyte4_attribute::ubyte4_attribute(std::vector<vector_type> const& values)
{
    m_values = values;
}

inline void ubyte4_attribute::set_at(size_t index, vector_type const& value)
{
    assert(index < m_values.size());
    m_values[index] = value;
}

inline vertex_element_type ubyte4_attribute::element_type() const
{
    return element_uint8;
}

inline size_t ubyte4_attribute::element_count() const
{
    return 4;
}

inline size_t ubyte4_attribute::count() const
{
    return m_values.size();
}

inline vector4d ubyte4_attribute::at(size_t index) const
{
    assert(index < m_values.size());
    vector<double,4> double_value
        = static_vector_cast<double>(m_values[index]);

    return vector4d(double_value);
}

}}}} // namespace pt::d3d9::rendering::source

#endif
