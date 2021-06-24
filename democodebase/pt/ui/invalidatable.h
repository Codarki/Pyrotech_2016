#ifndef PT_UI_INVALIDATABLE_H
#define PT_UI_INVALIDATABLE_H

namespace pt { namespace ui {

/// Abstract base class for invalidatable objects.
class invalidatable
{
public:
    virtual ~invalidatable() {}
    /// Invalidates this class, possibly causing paint event to be placed in
    /// the queue.
    void invalidate()
    {
        do_invalidate();
    }

private: /// virtuals which subclasses must implement
    virtual void do_invalidate() = 0;
};

}} // namespace pt::ui

#endif
