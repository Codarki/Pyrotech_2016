#ifndef PT_UI_ELEMENT_REGISTRY_H
#define PT_UI_ELEMENT_REGISTRY_H

#include "pt/boost/smart_ptr.h"
#include "pt/std/vector.h"
#include "pt/std/string.h"

namespace pt { namespace ui {

class element;
class element_type;

/// Singleton registry for element types. This includes all the elements from
/// all the projects. DEPRACATED \depracated
class element_registry
{
public:
    /// Returns the singleton instance of this registry.
    static element_registry& instance();

    /// \note Registry must be empty prior to destruction. \throw If existing
    /// registrations found.
    ~element_registry();

    /// Adds element \a type into this registerer. \throw If element type with
    /// same name already registered.
	void add(boost::shared_ptr<element_type> const& type);
    /// Removes element \a type from this registerer.
	void remove(boost::shared_ptr<element_type> const& type);

    /// \return New ui::element from type named \a name. \throw If not found.
    /// \throw If creation failed.
	boost::shared_ptr<element> create(std::string const& name) const;
    /// \return New ui::element from type named \a name. \return 0 if not
    /// found. \throw If creation failed.
	boost::shared_ptr<element> create_optional(std::string const& name) const;

	void clear()
	{
		m_types.clear();
	}

private: // type definitions
	typedef std::vector<boost::shared_ptr<element_type> > container;

private: // helper functions
    container::iterator find(std::string const& name, bool throw_if_not_found);
    container::const_iterator find(std::string const& name,
        bool throw_if_not_found) const;

private: // data members
    container m_types;
};

}} // namespace pt::ui

#endif
