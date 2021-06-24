#ifndef PT_D3D9_RENDERING_CAMERA_H
#define PT_D3D9_RENDERING_CAMERA_H

#include "view.h"
#include "composite_variable_owner.h"

namespace pt { namespace d3d9 { namespace rendering {

class camera : public view
{
public:
    camera(composite_variable_owner variables,
        std::shared_ptr<projection_matrix> projection,
        std::vector<animatable_value<math::plane>> clipping_planes,
        bool flip_culling_mode);

public: // interface required by d3d9::rendering::view
    projection_matrix& projection() override;
    variable_owner& variables() override;
    variable_owner const& variables() const override;
    std::shared_ptr<projection_matrix> shared_projection() const override;
    std::vector<animatable_value<math::plane>> const& clipping_planes() const override;
    bool flip_culling() const override;

private:
    camera const& operator=(camera const&) = delete;

private: // data members
    composite_variable_owner m_variables;
    std::shared_ptr<projection_matrix> m_projection;
    std::vector<animatable_value<math::plane>> m_clipping_planes;
    bool m_flip_culling_mode;
};

}}} // namespace pt::d3d9::rendering

#endif
