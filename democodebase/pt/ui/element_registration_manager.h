#ifndef PT_UI_ELEMENT_REGISTRATION_MANAGER_H
#define PT_UI_ELEMENT_REGISTRATION_MANAGER_H

#include "pt/boost/smart_ptr.h"
#include "pt/std/vector.h"

namespace pt { namespace ui {

class element_registration;

/// Collection of UI element type registrations. There is usually one object of
/// this manager per project. DEPRACATED \deprecated
class element_registration_manager
{
public:
    ~element_registration_manager();

	void add(boost::shared_ptr<element_registration> const& registration);

    /// Removes element \a registration from this manager. \throw If
    /// \a registration not found in this manager.
	void remove(boost::shared_ptr<element_registration> const& registration);

private: // type definitions
	typedef std::vector<boost::shared_ptr<element_registration> > container;

private: // data members
    container m_registrations;
};

}} // namespace pt::ui

#endif
