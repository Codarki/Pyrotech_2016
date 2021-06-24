#ifndef PT_D3D9_RENDERING_COMPOSITE_TRANSFORMATION_H
#define PT_D3D9_RENDERING_COMPOSITE_TRANSFORMATION_H

#include "transformation.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class composite_transformation final : public transformation
{
public:
	void push_back(std::shared_ptr<transformation> const& transformation);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const final override;
    bool get_is_constant() const final override;

private: // data members
    std::vector<std::shared_ptr<transformation> > m_transformations;
};

}}} // namespace pt::d3d9::rendering

#endif
