#ifndef PT_D3D9_RENDERING_SIMPLE_VIEW_H
#define PT_D3D9_RENDERING_SIMPLE_VIEW_H

#include "view.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 { namespace rendering {

//class simple_view : public view
//{
//public:
//    simple_view();
//
//    void set_local_to_world(matrix4x4f const& m);
//    void set_projection(matrix4x4f const& m);
//
//public: // interface required by view
//    matrix4x4f local_to_world_matrix() const;
//    matrix4x4f projection() const;
//    variable_owner& variables();
//    variable_owner const& variables() const;
//
//private:
//    matrix4x4f m_local_to_world;
//    matrix4x4f m_projection;
//    matrix4x4f m_world_to_local_clip;
//    std::shared_ptr<variable_owner> m_variables;
//};

}}} // namespace pt::d3d9::rendering

#endif
