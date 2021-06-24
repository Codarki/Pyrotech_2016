#include "constant_texture_2d_source.h"
#include "texture_2d_fwd.h"
#include "pt/string_wrapper.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 {

constant_texture_2d_source::constant_texture_2d_source()
{
}

constant_texture_2d_source::constant_texture_2d_source(std::shared_ptr<texture_2d> value)
: m_value(value)
{
}

void constant_texture_2d_source::set_texture(std::shared_ptr<texture_2d> texture)
{
    m_value = texture;
}

bool constant_texture_2d_source::is_constant() const
{
    return true;
}

std::shared_ptr<texture_2d> constant_texture_2d_source::value_at(float) const
{
    assert(m_value);
    return m_value;
}

string_wrapper constant_texture_2d_source::type_name() const
{
    return string_wrapper("constant_texture_2d");
}

}} // namespace pt::d3d9
