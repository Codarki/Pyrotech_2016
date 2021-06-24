#include "file_openmode.h"

namespace pt {
    
namespace openmode {
    file_openmode const in = (file_openmode)0x01;
    file_openmode const out = (file_openmode)0x02;
    file_openmode const ate = (file_openmode)0x04;
    file_openmode const append = (file_openmode)0x08;
    file_openmode const truncate = (file_openmode)0x10;
}

file_openmode operator|(file_openmode x, file_openmode y)
{
    return file_openmode(
        static_cast<unsigned int>(x) | static_cast<unsigned int>(y));
}

void operator|=(file_openmode& x, file_openmode const& y)
{
    x = x | y;
}

} // namespace pt
