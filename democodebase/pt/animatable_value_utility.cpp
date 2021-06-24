#include "animatable_value_utility.h"
#include "animatable_value_source.h"

namespace pt {

namespace {
    template<typename T>
    inline std::shared_ptr<animatable_value<T> > do_create_constant(T value)
    {
        std::shared_ptr<constant_value_source<T> > source(
            new constant_value_source<T>(value));

        return std::shared_ptr<animatable_value<T> >(new animatable_value<T>(source));
    }
}

std::shared_ptr<animatable_value<float> > create_constant_value(float value)
{
    return do_create_constant(value);
}

std::shared_ptr<animatable_value<int> > create_constant_value(int value)
{
    return do_create_constant(value);
}

std::shared_ptr<animatable_value<bool> > create_constant_value(bool value)
{
    return do_create_constant(value);
}

std::shared_ptr<animatable_value<std::string> > create_constant_value(
    std::string const& value)
{
    return do_create_constant(value);
}

std::shared_ptr<animatable_value<std::string> > create_constant_value(
    char const* value)
{
    return create_constant_value(std::string(value));
}

std::shared_ptr<animatable_value<uint32> > create_constant_value(uint32 value)
{
    return do_create_constant(value);
}

} // namespace pt
