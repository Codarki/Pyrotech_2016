#ifndef PT_CONVERSION_PROJECT_DESCRIPTION_H
#define PT_CONVERSION_PROJECT_DESCRIPTION_H

#include "pt/path.h"

namespace pt { namespace conversion {

class project_description
{
public:
    project_description(path source_directory, path intermediate_directory,
        path target_directory);

private: // data members
    path source_directory;
    path intermediate_directory;
    path target_directory;
};

}} // namespace pt::conversion

#endif
