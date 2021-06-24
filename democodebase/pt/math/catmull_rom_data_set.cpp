#include "catmull_rom_data_set.h"
#include "pt/debug_only.h"

namespace pt { namespace math {

// Free functions

data_set<float>::point_pair get_interval(float time,
    data_set<float> const& data)
{
    PT_DEBUG_ONLY(float tmp;)
    assert(!get_optional_endpoint(time, data, tmp));
    assert(data.size() > 1);
    data_set<float>::const_iterator current = data.begin();
    data_set<float>::const_iterator next = current + 1;

    //if(time < current->time)
    //{
    //    // Time before first key.
    //    current = data.end();
    //    next = data.begin();
    //    return data_set<float>::point_pair(current, next);
    //}

    while(next != data.end())
    {
        if(current->time == time)
        {
            // Exact match for key.
            next = data.end();
            return data_set<float>::point_pair(current, next);
        }

        if(current->time < time && time < next->time)
            return data_set<float>::point_pair(current, next);

        ++current;
        ++next;
    }
    assert(false);
    return data_set<float>::point_pair(data.end(), data.end());
    //// Time after last key.
    //current = data.end() - 1;
    //next = data.end() - 1;
    //return data_set<float>::point_pair(current, next);
}

bool get_optional_endpoint(float time, data_set<float> const& data,
    float& value)
{
    assert(data.size() > 0);
    if(data.size() == 1)
    {
        value = data.begin()->value;
        return true;
    }
    data_set<float>::const_iterator first = data.begin();

    if(time <= first->time)
    {
        value = first->value;
        return true;
    }

    data_set<float>::const_iterator last = data.end() - 1;
    if(time >= last->time)
    {
        value = last->value;
        return true;
    }
    return false;
}

}} // namespace pt::math
