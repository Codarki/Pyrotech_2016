#include "function_element_type.h"

namespace pt { namespace ui {

function_element_type::function_element_type(std::string const& element_name,
	function<boost::shared_ptr<element>()> creation_function)
:   m_name(element_name)
,   m_creation_function(creation_function)
{
}

std::string function_element_type::name() const
{
    return m_name;
}

boost::shared_ptr<element> function_element_type::create() const
{
    return m_creation_function();
}

}} // namespace pt::ui
