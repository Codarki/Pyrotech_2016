#ifndef PT_UI_ELEMENT_H
#define PT_UI_ELEMENT_H

#include "invalidatable.h"
#include "pt/config_node_fwd.h"

namespace pt { namespace ui {

class event;

/// Base class for UI elements that can receive events.
class element : public invalidatable
{
public:
    element();
    virtual ~element();

    /// Sets the \a parent for this element.
    void set_parent(element* parent);
    /// \return the \a parent for this element. \return 0 if this element does
    /// not have a parent.
    element* parent() const;

    /// Parses configuration for this UI element from \a config.
    void parse(config_node const& config);

public: // hook functions
    /// Global time for this element. All events directed to this element
    /// are transformed into elements local time. Default implementation
    /// returns 0. \return The global start time for this element.
    virtual float start_time() const;

private: // virtuals which subclasses must implement
    virtual void do_parse(config_node const& config);

private: // data members
    element* m_parent;
};

}} // namespace pt::ui

#endif
