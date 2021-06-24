#ifndef PT_WIN32_CLIPBOARD_H
#define PT_WIN32_CLIPBOARD_H

#include "pt/win32.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace win32 {

/// Windows clipboard.
///
/// Clipboard supports any number of formats. All of them are copied to/from
/// the Windows clipboard on operations. Formats include text, image, audio
/// or any application-defined data format.
///
/// Pasting text from the clipboard to the application.
/// \code
/// shared_ptr<clipboard_text> format(new clipboard_text);
///
/// clipboard clipboard(handle, format);
/// clipboard.paste();
///
/// log(format->text());
/// \endcode
///
/// Copying text from the application to the clipboard.
/// \code
/// clipboard clipboard(handle,
///     shared_ptr<clipboard_text>(new clipboard_text("my_text")));
/// clipboard.copy();
/// \endcode
///
class clipboard
{
public: // type definitions
    /// Clipboard data format.
    class format
    {
    public: // interface which subclasses must implement
        /// Copies data into the clipboard.
        virtual void copy() const = 0;
        /// Copies data from the clipboard.
        virtual void paste() = 0;

        /// Returns \c true if current clipboard contains data in this
        /// format.
        virtual bool is_available() const = 0;

        virtual ~format() {}
    };

public:
    clipboard(HWND handle);
    clipboard(HWND handle, std::shared_ptr<format> format);
    void add(std::shared_ptr<format> format);

    /// Copies data into the clipboard from all the formats in this class.
    void copy() const;
    /// Copies data from the clipboard to all the formats in this class.
    void paste();

private: // type definitions
	typedef std::vector<std::shared_ptr<format> > formats;

private: // data members
    formats m_formats;
    HWND m_handle;
};

}} // namespace pt::win32

#endif
