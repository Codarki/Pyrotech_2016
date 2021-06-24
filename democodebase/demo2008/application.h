#ifndef CONTENT_APPLICATION_H
#define CONTENT_APPLICATION_H

#include "pt/win32/message_pump.h"
#include "pt/command_line.h"
#include "pt/ui/elements/window.h"

namespace pt {
    class uncompressed_archive;
}

namespace content {

using namespace pt;

class demo_player;

class application : public win32::message_pump::listener
{
public:
    application(command_line const& commands, std::string demoname);
    ~application();

    void check_requirements();
    int run();

    static vector2i resolution()
    {
        return m_resolution;
    }

public: // interface required by win32::message_pump::listener
    bool processes_on_idle() const;
    float next_processing_delta_time() const;
    void on_idle();

private: // data members
	std::string m_demoname;
    ui::elements::window m_window;
	std::unique_ptr<win32::message_pump> m_pump;
	std::shared_ptr<demo_player> m_player;
    command_line m_commands;
    static vector2i m_resolution;

	std::shared_ptr<uncompressed_archive> m_archive;

    bool m_full_screen;
};

} // namespace content

#endif
