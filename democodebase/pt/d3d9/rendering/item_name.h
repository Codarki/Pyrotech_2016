#pragma once
#ifndef PT_D3D9_RENDERING_ITEM_NAME_H
#define PT_D3D9_RENDERING_ITEM_NAME_H

#include "pt/std/string.h"

namespace pt { namespace d3d9 { namespace rendering {

class item_name
{
public:
    item_name(std::string const& name)
    : m_name(name)
    {
    }
    item_name(char const name[])
        : m_name(name)
    {
    }
    std::string const& text() const
    {
        return m_name;
    }
private:
    std::string m_name;
};

inline bool operator==(item_name const& a, item_name const& b)
{
    return a.text() == b.text();
}

}}} // namespace pt::d3d9::rendering

#endif
