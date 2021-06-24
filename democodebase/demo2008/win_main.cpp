#include "win_main.h"
#include "application.h"
#include "pt/command_line.h"
#include "pt/log.h"
#include "pt/verbose.h"
#include "pt/win32.h"
#include "pt/path.h"
#include "pt/boost/lexical_cast.h"
#include "pt/boost/filesystem/convenience.h"
#include "pt_conversion/conversion.h"

bool convert()
{
    try
    {
        boost::filesystem::path source_directory
            = boost::filesystem::path("..") / boost::filesystem::path("src-data");

        pt::conversion::convert(source_directory.string(), "data");
    }
    catch(std::exception& e)
    {
        // doubt message box will work here. we dont have a window yet.
        int result = MessageBox(0, e.what(),
            "Errors during conversion, OK to continue?",
            MB_OKCANCEL | MB_ICONERROR | MB_SYSTEMMODAL);

        if (result != IDOK)
            return false;
    }
    return true;
}

// Entry point for windows application.
int APIENTRY WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
    LPTSTR lpCmdLine, int /*nCmdShow*/)
{
    // Set boost filesystem current directory.
    boost::filesystem::initial_path();

    //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);

    // Get the current bits
    int tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    // Clear the upper 16 bits and OR in the desired freqency
    tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_1024_DF;
    // Perform automatic leak checking at program exit
    tmp |= _CRTDBG_LEAK_CHECK_DF;

    // Set the new bits
    _CrtSetDbgFlag(tmp);

    //_CrtSetBreakAlloc(124520);
    // 124381

    std::string text;

    if (lpCmdLine != 0)
    {
        text = lpCmdLine;
    }

    if (text.empty())
    {
        text = "-serialize_only -res=1280x720 -timeline=data/timelines/unfault.xml";
    }

    pt::command_line cmd_line(lpCmdLine);

    try
    {
        if (cmd_line.has("verbose")) {
            // Enable logging.
            pt::enable_verbose(true);
        } else {
            pt::enable_verbose(false);
        }

        if (cmd_line.has("convert"))
        {
            if (!convert())
                return -1;
        }

        std::string demoname;

        if (cmd_line.has("demoname"))
        {
            cmd_line.try_get_value_of("demoname", demoname);
        }
        else
        {
            demoname = "antimon";
        }

        content::application app(cmd_line,demoname);
        app.check_requirements();
        return app.run();
    }
    catch(std::exception& e)
    {
        pt::log(e.what());
        return -1;
    }
}
