#ifndef PT_CONVERSION_CONVERTER_H
#define PT_CONVERSION_CONVERTER_H

#include "pt/std/memory.h"

namespace pt { namespace conversion {

class project;

class converter
{
public:
    converter(std::shared_ptr<conversion::project> project);

    void convert();

private: // data members
    std::shared_ptr<conversion::project> project;
};

}} // namespace pt::conversion

#endif
