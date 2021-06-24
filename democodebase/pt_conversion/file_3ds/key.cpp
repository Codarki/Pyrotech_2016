#include "key.h"

namespace pt { namespace conversion { namespace file_3ds {

key::acceleration_params::acceleration_params()
: tension(0.0f)
, continuity(0.0f)
, bias(0.0f)
, ease_to(0.0f)
, ease_from(0.0f)
{
}

key::key(int key_number, acceleration_params const& acceleration)
: m_key_number(key_number)
, m_acceleration(acceleration)
{
}

int key::key_number() const
{
    return m_key_number;
}

key::acceleration_params const& key::acceleration() const
{
    return m_acceleration;
}

}}} // namespace pt::conversion::file_3ds
