#include "dialog_box_error.h"
#include "pt/win32.h"
#include "pt/string_wrapper.h"
#include <Cderr.h>

namespace pt { namespace win32 {

namespace {
    /// \return \a error_code as human readable text.
    std::string human_readable_error_code(DWORD error_code)
    {
        switch(error_code)
        {
        // Common.
        case CDERR_DIALOGFAILURE:   return "CDERR_DIALOGFAILURE";
        case CDERR_FINDRESFAILURE:  return "CDERR_FINDRESFAILURE";
        case CDERR_NOHINSTANCE:     return "CDERR_NOHINSTANCE";
        case CDERR_INITIALIZATION:  return "CDERR_INITIALIZATION";
        case CDERR_NOHOOK:          return "CDERR_NOHOOK";
        case CDERR_LOCKRESFAILURE:  return "CDERR_LOCKRESFAILURE";
        case CDERR_NOTEMPLATE:      return "CDERR_NOTEMPLATE";
        case CDERR_LOADRESFAILURE:  return "CDERR_LOADRESFAILURE";
        case CDERR_STRUCTSIZE:      return "CDERR_STRUCTSIZE";
        case CDERR_LOADSTRFAILURE:  return "CDERR_LOADSTRFAILURE";
        case CDERR_MEMALLOCFAILURE: return "CDERR_MEMALLOCFAILURE";
        case CDERR_MEMLOCKFAILURE:  return "CDERR_MEMLOCKFAILURE";

        // PrintDlg() function.
        case PDERR_CREATEICFAILURE: return "PDERR_CREATEICFAILURE";
        case PDERR_DEFAULTDIFFERENT:return "PDERR_DEFAULTDIFFERENT";
        case PDERR_DNDMMISMATCH:    return "PDERR_DNDMMISMATCH";
        case PDERR_GETDEVMODEFAIL:  return "PDERR_GETDEVMODEFAIL";
        case PDERR_INITFAILURE:     return "PDERR_INITFAILURE";
        case PDERR_LOADDRVFAILURE:  return "PDERR_LOADDRVFAILURE";
        case PDERR_NODEFAULTPRN:    return "PDERR_NODEFAULTPRN";
        case PDERR_NODEVICES:       return "PDERR_NODEVICES";
        case PDERR_PARSEFAILURE:    return "PDERR_PARSEFAILURE";
        case PDERR_PRINTERNOTFOUND: return "PDERR_PRINTERNOTFOUND";
        case PDERR_RETDEFFAILURE:   return "PDERR_RETDEFFAILURE";
        case PDERR_SETUPFAILURE:    return "PDERR_SETUPFAILURE";

        // GetOpenFileName() and GetSaveFileName() functions.
        case FNERR_BUFFERTOOSMALL:  return "FNERR_BUFFERTOOSMALL";
        case FNERR_INVALIDFILENAME: return "FNERR_INVALIDFILENAME";
        case FNERR_SUBCLASSFAILURE: return "FNERR_SUBCLASSFAILURE";

        // ChooseFont() function.
        case CFERR_MAXLESSTHANMIN:  return "CFERR_MAXLESSTHANMIN";
        case CFERR_NOFONTS:         return "CFERR_NOFONTS";

        // FindText() and ReplaceText() functions.
        case FRERR_BUFFERLENGTHZERO:return "FRERR_BUFFERLENGTHZERO";
        default:
            return "unknown";
        };
    }

    /// \return Detailed error description for \a error_code.
    std::string detailed_error_text(DWORD error_code)
    {
        switch(error_code)
        {
        // Common.
        case CDERR_DIALOGFAILURE:
            return
                "The dialog box could not be created. The common dialog box "
                "function's call to the DialogBox function failed. For "
                "example, this error occurs if the common dialog box call "
                "specifies an invalid window handle.";

        case CDERR_FINDRESFAILURE:
            return "The common dialog box function failed to find a "
                "specified resource.";

        case CDERR_NOHINSTANCE:
            return
                "The ENABLETEMPLATE flag was set in the Flags member of the "
                "initialization structure for the corresponding common "
                "dialog box, but you failed to provide a corresponding "
                "instance handle.";

        case CDERR_INITIALIZATION:
            return
                "The common dialog box function failed during "
                "initialization. This error often occurs when sufficient "
                "memory is not available.";

        case CDERR_NOHOOK:
            return
                "The ENABLEHOOK flag was set in the Flags member of the "
                "initialization structure for the corresponding common "
                "dialog box, but you failed to provide a pointer to a "
                "corresponding hook procedure.";

        case CDERR_LOCKRESFAILURE:
            return "The common dialog box function failed to lock a "
                "specified resource.";

        case CDERR_NOTEMPLATE:
            return
                "The ENABLETEMPLATE flag was set in the Flags member of the "
                "initialization structure for the corresponding common "
                "dialog box, but you failed to provide a corresponding "
                "template.";

        case CDERR_LOADRESFAILURE:
            return "The common dialog box function failed to load a "
                "specified resource.";

        case CDERR_STRUCTSIZE:
            return "The lStructSize member of the initialization structure "
                "for the corresponding common dialog box is invalid.";

        case CDERR_LOADSTRFAILURE:
            return "The common dialog box function failed to load a "
                "specified string.";

        case CDERR_MEMALLOCFAILURE:
            return "The common dialog box function was unable to allocate "
                "memory for internal structures.";

        case CDERR_MEMLOCKFAILURE:
            return "The common dialog box function was unable to lock the "
                "memory associated with a handle.";

        // PrintDlg() function.
        case PDERR_CREATEICFAILURE:
            return "The PrintDlg function failed when it attempted to "
                "create an information context.";

        case PDERR_DEFAULTDIFFERENT:
            return
                "You called the PrintDlg function with the DN_DEFAULTPRN "
                "flag specified in the wDefault member of the DEVNAMES "
                "structure, but the printer described by the other "
                "structure members did not match the current default "
                "printer. (This error occurs when you store the DEVNAMES "
                "structure and the user changes the default printer by "
                "using the Control Panel.)"
                "\n"
                "To use the printer described by the DEVNAMES structure, "
                "clear the DN_DEFAULTPRN flag and call PrintDlg again."
                "\n"
                "To use the default printer, replace the DEVNAMES structure "
                "(and the structure, if one exists) with NULL; and call "
                "PrintDlg again.";

        case PDERR_DNDMMISMATCH:
            return "The data in the DEVMODE and DEVNAMES structures "
                "describes two different printers.";

        case PDERR_GETDEVMODEFAIL:
            return "The printer driver failed to initialize a DEVMODE "
                "structure.";

        case PDERR_INITFAILURE:
            return "The PrintDlg function failed during initialization, and "
                "there is no more specific extended error code to describe "
                "the failure. This is the generic default error code for "
                "the function.";

        case PDERR_LOADDRVFAILURE:
            return "The PrintDlg function failed to load the device driver "
                "for the specified printer.";

        case PDERR_NODEFAULTPRN:
            return "A default printer does not exist.";

        case PDERR_NODEVICES:
            return "No printer drivers were found.";

        case PDERR_PARSEFAILURE:
            return "The PrintDlg function failed to parse the strings in "
                "the [devices] section of the WIN.INI file.";

        case PDERR_PRINTERNOTFOUND:
            return "The [devices] section of the WIN.INI file did not "
                "contain an entry for the requested printer.";

        case PDERR_RETDEFFAILURE:
            return "The PD_RETURNDEFAULT flag was specified in the Flags "
                "member of the PRINTDLG structure, but the hDevMode or "
                "hDevNames member was not NULL.";

        case PDERR_SETUPFAILURE:
            return "The PrintDlg function failed to load the required "
                "resources.";

        // GetOpenFileName() and GetSaveFileName() functions.
        case FNERR_BUFFERTOOSMALL:
            return "The buffer pointed to by the lpstrFile member of the "
                "OPENFILENAME structure is too small for the file name "
                "specified by the user. The first two bytes of the "
                "lpstrFile buffer contain an integer value specifying the "
                "size, in TCHARs, required to receive the full name.";

        case FNERR_INVALIDFILENAME:
            return "A file name is invalid.";

        case FNERR_SUBCLASSFAILURE:
            return "An attempt to subclass a list box failed because "
                "sufficient memory was not available.";

        // ChooseFont() function.
        case CFERR_MAXLESSTHANMIN:
            return
                "The size specified in the nSizeMax member of the "
                "CHOOSEFONT structure is less than the size specified in "
                "the nSizeMin member.";

        case CFERR_NOFONTS:
            return "No fonts exist.";

        // FindText() and ReplaceText() functions.
        case FRERR_BUFFERLENGTHZERO:
            return "A member of the FINDREPLACE structure points to an "
                "invalid buffer.";

        default:
            return "unknown";
        };
    }
} // anonymous namespace

dialog_box_error::dialog_box_error(string_wrapper const& text)
{
    std::string error_text;
    DWORD result = CommDlgExtendedError();
    if (result == 0)
    {
        error_text = text.string() + ": user canceled";
    }
    else
    {
        error_text = text.string() + ": (" + human_readable_error_code(result) + ") "
            + detailed_error_text(result);
    }

    set_text(error_text);
}

}} // namespace pt::win32
