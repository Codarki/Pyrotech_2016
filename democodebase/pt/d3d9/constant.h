#ifndef PT_D3D9_CONSTANT_H
#define PT_D3D9_CONSTANT_H

#include "native.h"
#include "pt/com_ptr.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {

class constant;

/// Shader constant struct.
class constant_structure
{
public:
    size_t count() const;
    constant const& at(size_t index) const;
    constant const& by_name(std::string const& name) const;
    constant const* try_get(std::string const& name) const;

//protected:
    ~constant_structure();
    void push_back(constant const& constant);

private: // data members
    std::vector<constant> m_constants;
};

/// Shader constant. Constant is considered to be array of structures with at
/// least 1 element and 1 structure member.
class constant
{
public:
    constant();
    void populate(D3DXHANDLE handle, D3DXCONSTANT_DESC const& desc,
        com_ptr<ID3DXConstantTable> table);

    D3DXCONSTANT_DESC const& desc() const;
    std::string name() const;
    D3DXHANDLE handle() const;

    size_t element_count() const;
    bool is_structure() const;
    bool is_array() const;
    constant_structure const& at(size_t index) const;
    constant_structure const& operator[](size_t index) const;

    // Helper getter for structures of constants.
    constant const& by_name(std::string const& name) const;

private: // data members
    D3DXHANDLE m_handle;
    D3DXCONSTANT_DESC m_desc;
    std::vector<constant_structure> m_elements;
};

}} // namespace pt::d3d9

#endif
