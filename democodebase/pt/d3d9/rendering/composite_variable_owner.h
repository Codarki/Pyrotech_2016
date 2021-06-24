#ifndef PT_D3D9_RENDERING_COMPOSITE_VARIABLE_OWNER_H
#define PT_D3D9_RENDERING_COMPOSITE_VARIABLE_OWNER_H

#include "variable_owner.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class composite_variable_owner : public variable_owner
{
public:
    void push_back(std::shared_ptr<variable_owner> owner);

public: // interface required by variable_owner
    bool try_create_constants(constant_buffer const& buffer);
    void update_target(float time, constant_buffer const& buffer) const;

private: // data members
    std::vector<std::shared_ptr<variable_owner> > m_owners;
};

}}} // namespace pt::d3d9::rendering

#endif
