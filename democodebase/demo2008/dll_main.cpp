#include "application.h"
#include "pt/command_line.h"
#include "pt/log.h"
#include "pt/verbose.h"
#include "pt/win32.h"
#include "pt/boost/lexical_cast.h"
#include "pt/path.h"
#include "pt/boost/filesystem/convenience.h"

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

using namespace pt;
using namespace boost;

#pragma warning(disable: 4100)  // unreferenced formal parameter

__declspec(dllexport) int __cdecl run(int res_x, int res_y, BOOL windowed)
{
    // Set boost filesystem current directory.
    boost::filesystem::initial_path();

    pt::enable_verbose(false);

    std::string cmd_line_text
        = "-res=" + lexical_cast<std::string>(res_x)
        + "x" + lexical_cast<std::string>(res_y);
    cmd_line_text += " -timeline=data/timelines/2017.xml";
    if (windowed)
        cmd_line_text += " -window";

    cmd_line_text += " -serialize_only";

    pt::command_line cmd_line(cmd_line_text);

    try
    {
        std::string demoname;
        if (cmd_line.has("demoname"))
        {
            cmd_line.try_get_value_of("demoname", demoname);
        }
        else
        {
            demoname = "Pyrotech Demo";
        }

        content::application app(cmd_line, demoname);
        app.check_requirements();
        return app.run();
    }
    catch(std::exception& e)
    {
        pt::log(e.what());
        return -1;
    }
}

// Entry point for windows dll.
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch(reason) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#ifdef __cplusplus
}
#endif

