#ifndef PT_UI_FUNCTION_ELEMENT_TYPE_H
#define PT_UI_FUNCTION_ELEMENT_TYPE_H

#include "element_type.h"
#include "pt/boost/function.h"
#include "pt/boost/smart_ptr.h"
#include "pt/std/string.h"

namespace pt { namespace ui {

/// Generic element type that creates its elements by calling some function.
///
/// Following example code defines new ui::element and element type, which
/// defines its name and creation function:
/// \code
///
/// class my_element : public ui::element
/// {
///     ...
/// };
///
/// shared_ptr<element> my_create_function()
/// {
///     boost::shared_ptr<ui::element> new_element(new my_element);
///     return new_element;
/// }
///
/// shared_ptr<element_type> some_type(new function_element_type("my_element",
///     my_create_function);
///
/// \endcode
/// DEPRACATED \deprecated
class function_element_type : public element_type
{
public:
    /// Creates element type that associates \a creation_function with
    /// \a element_name. 
    function_element_type(std::string const& element_name,
        function<boost::shared_ptr<element> ()> creation_function);

public: // interface required by element_type
    std::string name() const;
	boost::shared_ptr<element> create() const;

private:
    std::string m_name;
	function<boost::shared_ptr<element>()> m_creation_function;
};

}} // namespace pt::ui

#endif
