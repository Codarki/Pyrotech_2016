#ifndef PT_D3D9_RENDERING_SIMPLE_TRANSFORMATION_H
#define PT_D3D9_RENDERING_SIMPLE_TRANSFORMATION_H

#include "transformation.h"
#include "pt/matrix.h"

namespace pt { namespace d3d9 { namespace rendering {

class simple_transformation : public transformation
{
public:
    simple_transformation();
    simple_transformation(matrix4x4f const& local_to_parent);
    void set_local_to_parent(matrix4x4f const& local_to_parent);

private: // interface required by transformation
    matrix4x4f get_local_to_parent_at(float time) const;
    bool get_is_constant() const;

private: // data members
    matrix4x4f m_local_to_parent;
};

// move to .cpp
inline simple_transformation::simple_transformation()
{
}
inline simple_transformation::simple_transformation(
    matrix4x4f const& local_to_parent)
:   m_local_to_parent(local_to_parent)
{
}
inline void simple_transformation::set_local_to_parent(
    matrix4x4f const& local_to_parent)
{
    m_local_to_parent = local_to_parent;
}
inline matrix4x4f simple_transformation::get_local_to_parent_at(float) const
{
    return m_local_to_parent;
}
inline bool simple_transformation::get_is_constant() const
{
    return false;
}

}}} // namespace pt::d3d9::rendering

#endif
