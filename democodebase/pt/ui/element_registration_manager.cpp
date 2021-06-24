#include "element_registration_manager.h"
#include "element_registration.h"
#include "element_type.h"
#include "pt/any_error.h"
#include "pt/format.h"

namespace pt { namespace ui {

element_registration_manager::~element_registration_manager()
{
}

void element_registration_manager::add(
	boost::shared_ptr<element_registration> const& registration)
{
    // element_registry performs name check that no duplicate element type
    // names exists.

    //container::iterator i = std::find(m_registrations.begin(),
    //    m_registrations.end(), registration);

    //if (i != m_registrations.end())
    //{
    //    throw any_error(
    //        format("registration for element type \"%1%\" already exist")
    //        % registration->type()->name());
    //}

    m_registrations.push_back(registration);
}

void element_registration_manager::remove(
	boost::shared_ptr<element_registration> const& registration)
{
    assert(registration);

    container::iterator i = std::find(m_registrations.begin(),
        m_registrations.end(), registration);

    if (i == m_registrations.end())
    {
        throw any_error(
            format("registration for element type \"%1%\" not found")
            % registration->type()->name());
    }

    m_registrations.erase(i);
}

}} // namespace pt::ui
