#include "simple_view.h"
#include "simple_variable_owner.h"
#include "constant_setter_utility.h"
#include "pt/d3d9/constant_buffer.h"
#include "pt/boost/noncopyable.h"
#include "pt/matrix_operations.h"

namespace pt { namespace d3d9 { namespace rendering {

//namespace {
//    class view_variables : public simple_variable_owner, boost::noncopyable
//    {
//    public:
//        view_variables(matrix4x4f const& local_to_world,
//            matrix4x4f const& projection,
//            matrix4x4f const& world_to_local_clip)
//        :   m_local_to_world(local_to_world)
//        ,   m_projection(projection)
//        ,   m_world_to_local_clip(world_to_local_clip)
//        {
//        }
//    protected: // interface required by simple_variable_owner
//        std::unique_ptr<constant_setter> on_create_constant(
//            constant const& constant) const
//        {
//            if (constant.name() == "projection")
//            {
//                return create_unique_reference_setter<matrix4x4f>(
//                    m_projection, constant);
//            }
//            else if (constant.name() == "projection")
//            {
//                return create_unique_reference_setter<matrix4x4f>(
//                    m_projection, constant);
//            }
//            else if (constant.name() == "world_to_view_clip")
//            {
//                return create_unique_reference_setter<matrix4x4f>(
//                    m_world_to_local_clip, constant);
//            }
//
//            return nullptr;
//        }
//    private: // data members
//        matrix4x4f const& m_local_to_world;
//        matrix4x4f const& m_projection;
//        matrix4x4f const& m_world_to_local_clip;
//    };
//} // anonymous namespace
//
//simple_view::simple_view()
//{
//    m_variables.reset(new view_variables(
//        m_local_to_world, m_projection, m_world_to_local_clip));
//}
//
//void simple_view::set_local_to_world(matrix4x4f const& m)
//{
//    m_local_to_world = m;
//
//    m_world_to_local_clip = invert(m_local_to_world) * m_projection;
//}
//
//void simple_view::set_projection(matrix4x4f const& m)
//{
//    m_projection = m;
//
//    m_world_to_local_clip = invert(m_local_to_world) * m_projection;
//}
//
//matrix4x4f simple_view::local_to_world_matrix() const
//{
//    return m_local_to_world;
//}
//
//matrix4x4f simple_view::projection() const
//{
//    return m_projection;
//}
//
//variable_owner& simple_view::variables()
//{
//    return *m_variables;
//}
//
//variable_owner const& simple_view::variables() const
//{
//    return *m_variables;
//}

}}} // namespace pt::d3d9::rendering
