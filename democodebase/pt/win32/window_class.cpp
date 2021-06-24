#include "window_class.h"
#include "win32_error.h"
#include "pt/format.h"

namespace pt { namespace win32 {
namespace {
    /// Returns \c ATOM that uniquely identifies the class.
    ATOM register_class(std::string const& name, std::string const& menu_name,
        HINSTANCE instance, wnd_proc window_procedure, HICON icon,
        HCURSOR cursor)
    {
        WNDCLASSEX desc;
        std::fill_n(reinterpret_cast<char*>(&desc), sizeof(desc), 0);

        desc.cbSize = sizeof(WNDCLASSEX);

        UINT style = 0;

        // I dont think we need own device context.
        //bool shared_device_context = false;
        //if (shared_device_context)
        //{
        //    // Allocates one device context to be shared by all windows in the
        //    // class.
        //    style |= CS_CLASSDC;
        //}
        //else
        //{
        //    // Allocates a unique device context for each window in the class.
        //    style |= CS_OWNDC;
        //}
        style |= CS_PARENTDC;
        // Sends a double-click message to the window procedure.
        style |= CS_DBLCLKS;
        // Redraws the entire window if a movement or size adjustment changes the
        // client area.
        style |= (CS_HREDRAW | CS_VREDRAW);
        desc.style = style;

        desc.lpfnWndProc = window_procedure;

        // no extra class memory
        desc.cbClsExtra = 0;
        // no extra window memory 
        desc.cbWndExtra = 0;
        desc.hInstance = instance;
        desc.hIcon = icon;
        desc.hCursor = cursor;
        desc.hIconSm = NULL;

        // If this parameter is set to NULL, the window must paint its own
        // background whenever it receives the WM_ERASEBKGND message
        desc.hbrBackground = 0;
        desc.lpszMenuName = menu_name.c_str();
        desc.lpszClassName = name.c_str();

        ATOM atom = RegisterClassEx(&desc);

        if (!atom)
            throw win32_error(format("RegisterClassEx: \"%1%\"") % name);

        return atom;
    }
} // anonymous namespace

window_class::window_class(std::string const& name,
    std::string const& menu_name, HINSTANCE instance,
    wnd_proc window_procedure, HICON icon, HCURSOR cursor)
:   m_instance(instance)
{
    m_atom = register_class(name, menu_name, m_instance, window_procedure,
        icon, cursor);
}

window_class::window_class(std::string const& name,
    std::string const& menu_name, HINSTANCE instance,
    wnd_proc window_procedure)
:   m_instance(instance)
{
    HICON icon = LoadIcon(NULL, IDI_APPLICATION);
    HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);

    m_atom = register_class(name, menu_name, m_instance, window_procedure,
        icon, cursor);
}

window_class::~window_class()
{
    //BOOL result =
		UnregisterClass(reinterpret_cast<LPCTSTR>(atom()), m_instance);

    //if (!result)
    //    throw win32_error("UnregisterClass");
}

ATOM window_class::atom() const
{
    return m_atom;
}

HINSTANCE window_class::instance() const
{
    return m_instance;
}

}} // namespace pt::win32
