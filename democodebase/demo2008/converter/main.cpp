#include "pt_conversion/conversion.h"
#include "pt/command_line.h"
#include "pt/string_builder.h"
#include "pt/boost/filesystem/convenience.h"

void convert()
{
    try
    {
        pt::conversion::convert("..//src-data", "data");
    }
    catch (std::exception&)
    {
        // TODO
    }
}

// Default debug working directory: democodebase\\demo2008\\converter
//
// Options:
// -SourceDir source_directory
// -IntermediateDir intermediate_directory
// -TargetDir target_directory
//
// convert -SourceDir=./src-data -IntermediateDir=./intermediate-data
// convert -IntermediateDir=./intermediate-data -TargetDir=./data
//
// ../../../demo2014/src-data
// ../../../demo2014/intermediate-data
// ../../../demo2014/exe/data
// convert -SourceDir=../../../demo2014/src-data -IntermediateDir=../../../demo2014/intermediate-data
// convert -SourceDir ../../../demo2014/src-data -IntermediateDir ../../../demo2014/intermediate-data
// convert /SourceDir ../../../demo2014/src-data /IntermediateDir ../../../demo2014/intermediate-data
// convert /SourceDir "../../../demo2014/src-data" /IntermediateDir "../../../demo2014/intermediate-data"

std::string text_from(int argc, char* argv[])
{
    std::string result;
    for (int i = 1; i < argc; ++i)
    {
        if (i > 1)
            result += " ";
        result += pt::to_lower_case(argv[i]);
    }
    return result;
}

int main(int argc, char* argv[])
{
    boost::filesystem::initial_path();

    pt::command_line command_line(text_from(argc, argv));

    pt::conversion::conversion_settings settings;
    if (command_line.has("SourceDir"))
    {
        settings.source_directory = command_line.value_of("SourceDir");
    }
    if (command_line.has("IntermediateDir"))
    {
        settings.intermediate_directory = command_line.value_of("IntermediateDir");
    }
    if (command_line.has("TargetDir"))
    {
        settings.target_directory = command_line.value_of("TargetDir");
    }

    convert();
}
