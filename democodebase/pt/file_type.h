#pragma once
#ifndef PT_FILE_TYPE_H
#define PT_FILE_TYPE_H

#include "stream_fwd.h"

namespace pt {

class file_type
{
public:
    file_type(bool is_binary)
    : m_is_binary(is_binary)
    {
    }

    bool is_binary() const
    {
        return m_is_binary;
    }

private: // data members
    bool m_is_binary;
};

} // namespace pt

#endif
