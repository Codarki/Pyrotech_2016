#pragma once
#ifndef PT_D3D9_RENDERING_TIME_SETTER_H
#define PT_D3D9_RENDERING_TIME_SETTER_H

#include "constant_setter.h"
#include "pt/d3d9/constant_utility.h"

namespace pt { namespace d3d9 { namespace rendering {

class time_setter : public constant_setter
{
public:
public: // interface required by variable_setter
    void update_target(float time) const
    {
        set_constant(constant(), constants(), time);
    }

private: // nonassignable
    time_setter const& operator=(time_setter const&);
};

}}} // namespace pt::d3d9::rendering

#endif
