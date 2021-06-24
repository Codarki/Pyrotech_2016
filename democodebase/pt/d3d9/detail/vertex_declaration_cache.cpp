#include "vertex_declaration_cache.h"
#include "../error.h"
#include "../graphics_device.h"
#include "pt/any_error.h"

namespace pt { namespace d3d9 { namespace detail {

namespace {
    bool equals(D3DVERTEXELEMENT9 const& a, D3DVERTEXELEMENT9 const& b)
    {
        int result = ::memcmp(&a, &b, sizeof(D3DVERTEXELEMENT9));
        return result == 0;
    }

    size_t element_count(D3DVERTEXELEMENT9 const* elements)
    {
        size_t result = 0;
        D3DVERTEXELEMENT9 end = D3DDECL_END();

        while(!equals(*elements, end))
        {
            ++elements;
            ++result;
        }
        return result + 1;
        //throw any_error("no elements");
    }
} // anonymous namespace

vertex_declaration_cache::vertex_declaration_cache(
    graphics_device const& device)
:   m_device(device)
{
}

com_ptr<IDirect3DVertexDeclaration9> vertex_declaration_cache::find_or_create(
    D3DVERTEXELEMENT9 const* elements) const
{
    size_t count = element_count(elements);

    // Find.
    for (std::vector<declaration>::const_iterator i = m_declarations.begin();
        i != m_declarations.end(); ++i)
    {
        if (count != i->elements.size())
            continue;

        bool mismatch = false;

        for (size_t j = 0; j < i->elements.size(); ++j)
        {
            if (!equals(i->elements[j], elements[j]))
            {
                mismatch = true;
                break;
            }
        }

        if (!mismatch)
            return i->native;
    }

    // Create.
    declaration d;

    check_dx(m_device.mutable_native()->CreateVertexDeclaration(elements,
        d.native.receive()),
        "IDirect3DDevice9::CreateVertexDeclaration");

    d.elements.resize(count);
    for (size_t i = 0; i < count; ++i)
        d.elements[i] = elements[i];

    m_declarations.push_back(d);

    return d.native;
}

}}} // namespace pt::d3d9::detail
