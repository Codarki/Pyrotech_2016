#ifndef PT_WIN32_MESSAGE_PUMP_H
#define PT_WIN32_MESSAGE_PUMP_H

#include "pt/defs.h"
#include "pt/boost/optional.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"
#include "pt/std/string.h"

namespace pt { namespace win32 {

/// Interface for message pump quit reguest.
class message_pump_quit_request
{
public:
    /// Informs the message pump that we are about to quit. After control is
    /// returned to the message pump, it will exit the run() function.
    virtual void quit() = 0;
    virtual void quit(int return_code, std::string const& reason) = 0;
};

/// Windows message pump.
class message_pump : public message_pump_quit_request
{
public: // type definitions
    /// Interface for classes that want to process some work when message
    /// queue is idle (empty of windows messages).
    class listener
    {
    public:
        /// Query if message queue should call this listener's on_idle()
        /// when module's message queue is empty. \return \c true if on_idle()
        /// should be called.
        virtual bool processes_on_idle() const = 0;

        /// Returns the elapsed time that this listener expects to be called
        /// again. Thread goes to sleep for until next listener needs to be
        /// called.
        virtual float next_processing_delta_time() const = 0;

        /// Processes continuous workload when message queue is empty.
        virtual void on_idle() = 0;
        virtual ~listener() {}
    };
public:
    message_pump();

    /// Adds \a listener to this message pump.
    void add_listener(std::shared_ptr<listener> const& listener);

    /// Runs this message pump until application quits.
    /// \return \c 0 if normal exit. \return Error code if error occured.
    int run();
    int run(listener& listener);
    int run_pending_messages();

    bool is_running() const;

public: // interface required by message_pump_quit_request
    void quit();
    void quit(int return_code, std::string const& reason);

private: // type definitions
    typedef std::vector<std::shared_ptr<listener> > container;

private: // helper functions
    boost::optional<int> pump_message();
    bool has_idle_processing_listener() const;
    float next_idle_processing_time() const;

private: // data members
    container m_listeners;
    bool m_quit_requested;
    bool m_is_running;
    int m_return_code;
    std::string m_quit_reason;
};

}} // namespace pt::win32

#endif
