#ifndef PT_D3D9_SEMANTIC_H
#define PT_D3D9_SEMANTIC_H

#include "pt/std/string.h"

namespace pt { namespace d3d9 {

class semantic
{
public:
    semantic(std::string const& name);
    semantic(std::string const& name, size_t index);

    std::string text() const;

    std::string const& name() const;
    size_t index() const;

    bool operator==(semantic const& x) const;
    bool operator<(semantic const& x) const;

private:
    std::string m_name;
    size_t m_index;
};

}} // namespace pt::d3d9

#endif
