#ifndef PT_D3D9_DETAIL_VERTEX_DECLARATION_CACHE_H
#define PT_D3D9_DETAIL_VERTEX_DECLARATION_CACHE_H

#include "pt/d3d9/native.h"
#include "pt/d3d9/graphics_device_fwd.h"
#include "pt/com_ptr.h"
#include "pt/boost/noncopyable.h"
#include "pt/std/map.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace detail {

class vertex_declaration_cache : boost::noncopyable
{
public:
    vertex_declaration_cache(graphics_device const& device);

    com_ptr<IDirect3DVertexDeclaration9> find_or_create(
        D3DVERTEXELEMENT9 const* elements) const;

private: // type definitions
    struct declaration
    {
        std::vector<D3DVERTEXELEMENT9> elements;
        com_ptr<IDirect3DVertexDeclaration9> native;
    };

private: // data members
    graphics_device const& m_device;
    mutable std::vector<declaration> m_declarations;
};

}}} // namespace pt::d3d9::detail

#endif
