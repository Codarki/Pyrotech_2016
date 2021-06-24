#include "dll.h"
#include "win32_error.h"
#include "pt/log.h"
#include "pt/path.h"
#include "pt/string_wrapper.h"
#include "pt/win32.h"
#include "pt/boost/filesystem/convenience.h"
#include "pt/boost/format.h"
#include "pt/boost/noncopyable.h"

namespace pt { namespace win32 {

//using boost::shared_ptr;

namespace detail {
    class native_dll : boost::noncopyable
    {
    public:
        native_dll(pt::path const& path)
        :   m_path(path)
        {
            assert(m_path.is_absolute());

            std::string path_string = m_path.string();
            //HMODULE test = GetModuleHandle(path_string.c_str());
            //if (!test)
            //{
            //    log(format("module %1% already loaded in this process")
            //        % path_string.c_str());
            //}

            auto path_before = boost::filesystem::current_path();

            HANDLE heap_before = GetProcessHeap();

            m_handle = LoadLibrary(path_string.c_str());

            HANDLE heap_after = GetProcessHeap();
            auto path_after = boost::filesystem::current_path();

            if (heap_before != heap_after)
            {
                log("heap changed");
            }

            if (path_before != path_after)
            {
                log("current path changed");
            }

            if (!m_handle)
                throw win32_error("LoadLibrary " + path_string);
        }

        pt::path const& path() const
        {
            return m_path;
        }

        ~native_dll()
        {
            FreeLibrary(m_handle);
        }

        void* get_proc_address(std::string const& function_name) const
        {
            void* address = GetProcAddress(m_handle, function_name.c_str());

            if (!address)
                throw win32_error("GetProcAddress " + function_name);

            return address;
        }

    private: // data members
        HMODULE m_handle;
        pt::path m_path;
    };

	std::vector<std::weak_ptr<native_dll> > g_process_dlls;

    /// This function will share the actual DLL's when loaded multiple times
    /// in a process.
    std::shared_ptr<native_dll> find_or_load_dll(path const& path_)
    {
        path complete_path
            = pt::absolute(path_, pt::initial_path());

		for (std::vector<std::weak_ptr<native_dll> >::iterator i
            = g_process_dlls.begin(); i != g_process_dlls.end(); ++i)
        {
            std::shared_ptr<native_dll> dll = i->lock();
            if (!dll)
            {
                // Remove dead DLL.
                g_process_dlls.erase(i);
                continue;
            }

            if (dll->path() == complete_path)
                return dll;
        }

        // Load new DLL.
        std::shared_ptr<native_dll> dll(new native_dll(complete_path));
        g_process_dlls.push_back(dll);
        return dll;
    }
} // namespace detail

dll::~dll()
{
}

simple_dll::simple_dll(path const& path)
:   m_impl(detail::find_or_load_dll(path))
{
}

void* simple_dll::get_proc_address(std::string const& function_name) const
{
    assert(m_impl);
    return m_impl->get_proc_address(function_name);
}

}} // namespace pt::win32
