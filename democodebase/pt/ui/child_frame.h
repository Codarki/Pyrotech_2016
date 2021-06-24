#ifndef PT_UI_CHILD_FRAME_H
#define PT_UI_CHILD_FRAME_H

#include "pt/box.h"
#include "pt/matrix.h"
#include "pt/oriented_box.h"

namespace pt { namespace ui {

/// Abstract interface for UI coordinate frames.
class child_frame
{
public:
    virtual ~child_frame();

public:
    /// Sets local-to-parent \a matrix.
    virtual void set_local_to_parent_matrix(matrix3x3f const& matrix) = 0;
    /// \return local-to-parent \a matrix.
    virtual matrix3x3f const& local_to_parent_matrix() const = 0;
};

/// Abstract interface for UI coordinate frames which has bounds.
class bounded_frame : public child_frame
{
public: // interface which subclasses must implement
    /// Sets \a bounds for this element in local coordinate space.
    virtual void set_bounds(box2i const& bounds) = 0;
    /// \return Bounds of this element in local coordinate space.
    virtual box2i const& bounds() const = 0;
};

/// Simple bounded element.
class simple_bounded_frame : public bounded_frame
{
public:
    simple_bounded_frame();
    simple_bounded_frame(box2i const& bounds,
        matrix3x3f const& local_to_parent);

public: // interface required by child_frame
    void set_local_to_parent_matrix(matrix3x3f const& matrix);
    matrix3x3f const& local_to_parent_matrix() const;

public: // interface required by bounded_frame
    void set_bounds(box2i const& bounds);
    box2i const& bounds() const;

protected: // hook functions
    /// Called when set_bounds() is called. Default implementation does
    /// nothing.
    virtual void set_bounds_called();

private: // data members
    box2i m_bounds;
    matrix3x3f m_local_to_parent;
};

/// \return Bounds of \a child in its parents coordinate space.
oriented_box2i bounds_in_parent(bounded_frame const& child);

}} // namespace pt::ui

#endif
