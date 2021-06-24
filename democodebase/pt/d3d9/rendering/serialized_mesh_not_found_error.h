#ifndef PT_D3D9_RENDERING_SERIALIZED_MESH_NOT_FOUND_ERROR_H
#define PT_D3D9_RENDERING_SERIALIZED_MESH_NOT_FOUND_ERROR_H

#include "pt/format.h"
#include "pt/exceptions/contextual_exception.h"

namespace pt { namespace d3d9 { namespace rendering {

class serialized_mesh_not_found_error : public pt::exceptions::contextual_exception
{
public:
    serialized_mesh_not_found_error(std::string const& file)
    {
        this->set_text(pt::format("Serialized mesh not found: %1%") % file);
    }
};

}}} // namespace pt::d3d9::rendering

#endif
