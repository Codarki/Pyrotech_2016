#ifndef PT_CONVERSION_PROJECT_H
#define PT_CONVERSION_PROJECT_H

#include "pt/std/memory.h"

namespace pt { namespace conversion {

class project_description;

class project
{
public:
    project(std::unique_ptr<project_description>&& description);

private: // data members
    std::unique_ptr<project_description> description;
};

}} // namespace pt::conversion

#endif
