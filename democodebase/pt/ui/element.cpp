#include "element.h"

namespace pt { namespace ui {

element::element()
:   m_parent(0)
{
}

element::~element()
{
}

void element::set_parent(element* parent)
{
    m_parent = parent;
}

element* element::parent() const
{
    return m_parent;
}

void element::parse(config_node const& config)
{
    do_parse(config);
}

float element::start_time() const
{
    return 0.0f;
}

void element::do_parse(config_node const&)
{
}

}} // namespace pt::ui
