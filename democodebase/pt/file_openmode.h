#pragma once
#ifndef PT_FILE_OPEN_MODE_H
#define PT_FILE_OPEN_MODE_H

namespace pt {

enum file_openmode
{
    file_openmode_mask = 0xff
};

namespace openmode {
    extern file_openmode const in;
    extern file_openmode const out;
    extern file_openmode const ate;
    extern file_openmode const append;
    extern file_openmode const truncate;
}

file_openmode operator|(file_openmode x, file_openmode y);
void operator|=(file_openmode& x, file_openmode const& y);

} // namespace pt

#endif
