#ifndef PT_UI_PARENTED_ELEMENT_H
#define PT_UI_PARENTED_ELEMENT_H

#include "element.h"

namespace pt { namespace ui {

/// Element class that notifies parent when this element is being
/// invalidated. \note DEPRACATED \depracated
class parented_element : public element
{
public:
    /// Constructs an element with a \a parent. Does not hold reference to
    /// parent.
    parented_element(element* parent);

public: // interface required by invalidatable
    void invalidate();

protected: // hook functions
    /// Hook function called from invalidate(). Default implementation does
    /// nothing. Override this if you want to be notified in subclass.
    virtual void on_invalidate();

private: // data members
    element* m_parent;
};

}} // namespace pt::ui

#endif
