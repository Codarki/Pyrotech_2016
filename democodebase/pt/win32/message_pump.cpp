#include "message_pump.h"
#include "win32_error.h"
#include "pt/log.h"
#include "pt/scoped_assignment.h"
#include "pt/win32.h"
#include "pt/std/string.h"
#include <boost/bind.hpp>
#include <limits>

namespace pt { namespace win32 {
namespace {
    /// Returns true if there are messages in queue.
    bool has_messages()
    {
        MSG msg;
        return PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) != 0;
    }
} // anonymous namespace

message_pump::message_pump()
:   m_is_running(false)
{
}

void message_pump::add_listener(std::shared_ptr<listener> const& listener)
{
    m_listeners.push_back(listener);
}

int message_pump::run()
{
    m_quit_requested = false;
    m_return_code = 0;

    while(!m_quit_requested)
    {
        // Process on idle until there is new message(s) waiting.
        while(!has_messages() && has_idle_processing_listener())
        {
            // Poll and wait for next idle processing.
            DWORD next_time_ms = static_cast<DWORD>(
                next_idle_processing_time() * 1000);

            // feel free to disable this logging, if long sleep is intended.
            if (next_time_ms > 1000)
                log("Warning: thread sleeping over 1 second.");

            if (next_time_ms > 0)
                Sleep(next_time_ms);

            std::for_each(m_listeners.begin(), m_listeners.end(),
                boost::bind(&listener::on_idle, _1));
        }

        boost::optional<int> result = pump_message();

        if (result)
            return *result;
    }
    return m_return_code;
}

int message_pump::run(listener& listener)
{
    m_quit_requested = false;
    m_return_code = 0;

    scoped_assignment<bool> running(m_is_running, true);

    while(!m_quit_requested)
    {
        // Process on idle until there is new message(s) waiting.
        while(!has_messages() && listener.processes_on_idle())
        {
            // Poll and wait for next idle processing.
            DWORD next_time_ms = static_cast<DWORD>(
                listener.next_processing_delta_time() * 1000);

            // feel free to disable this logging, if long sleep is intended.
            if (next_time_ms > 1000)
                log("Warning: thread sleeping over 1 second.");

            if (next_time_ms > 0)
                Sleep(next_time_ms);

            listener.on_idle();

            if (m_quit_requested)
                return m_return_code;
        }

        boost::optional<int> result = pump_message();

        if (result)
            return *result;
    }
    return m_return_code;
}

int message_pump::run_pending_messages()
{
    m_quit_requested = false;
    m_return_code = 0;

    scoped_assignment<bool> running(m_is_running, true);

    while(has_messages() && !m_quit_requested)
    {
        boost::optional<int> result = pump_message();

        if (result)
            return *result;
    }
    return m_return_code;
}

bool message_pump::is_running() const
{
    return m_is_running;
}

void message_pump::quit()
{
    m_quit_requested = true;
}

void message_pump::quit(int return_code, std::string const& reason)
{
    quit();
    m_return_code = return_code;
    m_quit_reason = reason;
}

/// Pumps single message from windows message queue and dispatches it to
/// windows procedure, which was registered in the win32::window_class.
boost::optional<int> message_pump::pump_message()
{
    MSG msg;
    BOOL result = GetMessage(&msg, NULL, 0, 0);
    
    if (result == 0)
    {
        // GetMessage returns 0 when WM_QUIT
        if (m_return_code != 0)
            log("overriding message pump return code");
        m_return_code = static_cast<int>(msg.wParam);
        return boost::optional<int>(m_return_code);
    }

    if (result == -1)
        throw win32_error("GetMessage");

    // Translates virtual key codes to characters.
    TranslateMessage(&msg); 
    // Dispatch message to the window procedure.
    DispatchMessage(&msg);

    return boost::optional<int>();
}

/// \return \c true if any of the listeners want to process on idle.
bool message_pump::has_idle_processing_listener() const
{
    for (container::const_iterator i = m_listeners.begin();
        i != m_listeners.end(); ++i)
    {
        if ((*i)->processes_on_idle())
            return true;
    }
    return false;
}

/// Returns lowest delta time in seconds, that thread can sleep, until it is
/// expected to call some listener.
float message_pump::next_idle_processing_time() const
{
    float dt = std::numeric_limits<float>::max();

    for (container::const_iterator i = m_listeners.begin();
        i != m_listeners.end(); ++i)
    {
        dt = std::min(dt, (*i)->next_processing_delta_time());
    }
    return dt;
}

}} // namespace pt::win32
