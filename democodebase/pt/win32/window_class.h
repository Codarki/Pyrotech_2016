#ifndef PT_WIN32_WINDOW_CLASS_H
#define PT_WIN32_WINDOW_CLASS_H

#include "pt/defs.h"
#include "pt/win32.h"
#include "pt/std/string.h"

namespace pt { namespace win32 {

/// Alias for default window procedure.
typedef WNDPROC wnd_proc;

/// Represents registered application local window class.
/// An application local class is any window class that an executable or .dll
/// registers for its exclusive use. Window class has an associated window
/// procedure shared by all windows of the same class.
///
/// Snippet from msdn:
/// Windows NT/Windows 2000/Windows XP: The class is not destroyed when the
/// .dll that owns it is unloaded. Therefore, if the system calls the window
/// procedure for a window of this class, it will cause an access violation,
/// because the .dll containing the window procedure is no longer in memory.
/// The process must destroy all windows using the class before the .dll is
/// unloaded and call the UnregisterClass() function.
///
class window_class
{
public:
    /// Registers window class for module \a instance with window \a name
    /// and \a window_procedure.
    ///
    /// \param window_procedure Function pointer to window procedure.
    ///
    window_class(std::string const& name, std::string const& menu_name,
        HINSTANCE instance, wnd_proc window_procedure, HICON icon,
        HCURSOR cursor);
    /// \overload
    window_class(std::string const& name, std::string const& menu_name,
        HINSTANCE instance, wnd_proc window_procedure);

    /// Unregisters this class. Before calling this function, an application
    /// must destroy all windows created with this class.
    ~window_class();

    /// Returns \c ATOM that uniquely identifies this class.
    ATOM atom() const;

    /// Returns handle of the module (exe or dll) this class was registered to.
    HINSTANCE instance() const;

private: // data members
    ATOM m_atom;
    HINSTANCE m_instance;
};

}} // namespace pt::win32

#endif
