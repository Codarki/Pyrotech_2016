#ifndef PT_UI_ELEMENT_TYPE_H
#define PT_UI_ELEMENT_TYPE_H

#include "pt/boost/smart_ptr.h"
#include "pt/std/string.h"

namespace pt { namespace ui {

class element;

/// Abstract base class for all element types. Element type provides an unique
/// name and creation function for each type of ui elements.
class element_type
{
public:
    virtual std::string name() const = 0;
	virtual boost::shared_ptr<element> create() const = 0;
};

}} // namespace pt::ui

#endif
