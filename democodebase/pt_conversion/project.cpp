#include "project.h"
#include "project_description.h"
#include "converter.h"

namespace pt { namespace conversion {

// project_description

project_description::project_description(
    path source_directory,
    path intermediate_directory,
    path target_directory)
: source_directory(source_directory)
, intermediate_directory(intermediate_directory)
, target_directory(target_directory)
{
}

// project

project::project(std::unique_ptr<project_description>&& description)
: description(move(description))
{
}

// converter

converter::converter(std::shared_ptr<conversion::project> project)
{
}

void converter::convert()
{
}

}} // namespace pt::conversion
