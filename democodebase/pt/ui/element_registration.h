#ifndef PT_UI_ELEMENT_REGISTRATION_H
#define PT_UI_ELEMENT_REGISTRATION_H

#include "pt/boost/smart_ptr.h"

namespace pt { namespace ui {

class element_type;

/// Utility class that inserts UI element type into ui::element_registry on
/// construction and removes it on destruction. DEPRACATED \deprecated
class element_registration
{
public:
    /// Registers element \a type. \throw If same named element type is already
    /// registered.
	element_registration(boost::shared_ptr<element_type> const& type);
    /// Unregisters element type associated with this registration.
    ~element_registration();

	boost::shared_ptr<element_type const> type() const;

private: // data members
	boost::shared_ptr<element_type> m_type;
};

}} // namespace pt::ui

#endif
