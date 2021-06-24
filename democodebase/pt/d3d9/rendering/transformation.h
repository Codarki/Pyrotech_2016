#ifndef PT_D3D9_RENDERING_TRANSFORMATION_H
#define PT_D3D9_RENDERING_TRANSFORMATION_H

#include "pt/matrix.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace rendering {

class transformation
{
public:
    transformation();
    virtual ~transformation();

	void set_parent(std::shared_ptr<transformation const> parent);
    std::shared_ptr<transformation const> const& parent() const;

    matrix4x4f const& local_to_world_at(float time) const;
    matrix4x4f const& local_to_parent_at(float time) const;

    bool is_constant() const;

protected:
    void invalidate();

private: // interface which subclasses must implement
    virtual matrix4x4f get_local_to_parent_at(float time) const = 0;
    virtual bool get_is_constant() const = 0;

private: // data members
    std::shared_ptr<transformation const> m_parent;
    mutable float m_local_to_parent_time;
    mutable float m_local_to_world_time;
    mutable matrix4x4f m_local_to_parent;
    mutable matrix4x4f m_local_to_world;
};

/// Returns transformation matrix from \a child's local space to \a parent's
/// local space. If \a parent is not parent of \a child, then local-to-world
/// matrix is returned.
matrix4x4f local_to_parent(transformation const& child,
    transformation const* parent, float time);

}}} // namespace pt::d3d9::rendering

#endif
