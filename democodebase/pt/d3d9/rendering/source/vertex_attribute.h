#ifndef PT_D3D9_RENDERING_SOURCE_VERTEX_ATTRIBUTE_H
#define PT_D3D9_RENDERING_SOURCE_VERTEX_ATTRIBUTE_H

#include "../../enum_utility.h"
#include "pt/vector.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

/// Container of single vertex attribute. ie vertex positions.
///
/// Purpose of this class is to act as source data for vertex buffers.
class vertex_attribute
{
public:
    virtual ~vertex_attribute() {}

public: // interface which subclasses must implement
    /// Returns element type for this attribute. ie float in vector4f.
    virtual vertex_element_type element_type() const = 0;
    /// Returns amount of elements in single attribute. ie 4 in vector4f.
    virtual size_t element_count() const = 0;
    /// Returns amount of values in this attribute.
    virtual size_t count() const = 0;
    /// Returns value at \a index.
    virtual vector4d at(size_t index) const = 0;
};

/// Vertex attributes of float vector of \c dim dimensions.
template<int dim>
class vectorf_attribute : public vertex_attribute
{
public: // type definitions
    typedef vector<float,dim> vector_type;

public:
    vectorf_attribute(size_t count);
    vectorf_attribute(std::vector<vector_type> const& values);
    void set_at(size_t index, vector<float,dim> const& value);

public: // interface required by vertex_attribute
    vertex_element_type element_type() const;
    size_t element_count() const;
    size_t count() const;
    vector4d at(size_t index) const;

private: // data members
    std::vector<vector<float,dim> > m_values;
};

/// Vertex attributes of four unsigned bytes.
class ubyte4_attribute : public vertex_attribute
{
public: // type definitions
    typedef vector<unsigned char,4> vector_type;

public:
    ubyte4_attribute(size_t count);
    ubyte4_attribute(std::vector<vector_type> const& values);
    void set_at(size_t index, vector_type const& value);

public: // interface required by vertex_attribute
    vertex_element_type element_type() const;
    size_t element_count() const;
    size_t count() const;
    vector4d at(size_t index) const;

private: // data members
    std::vector<vector_type> m_values;
};

}}}} // namespace pt::d3d9::rendering::source

#include "vertex_attribute.inl"

#endif
