#include "system.h"
#include "fmod_error.h"
#include "sound.h"
#include "pt/file_input_stream.h"
#include "pt/data_stream_content.h"
#include "pt/path.h"

namespace fmod_impl {

system::system()
{
    check_fmod(FMOD::System_Create(&m_native), "FMOD::System_Create");

    FMOD_INITFLAGS flags = FMOD_INIT_NORMAL;// | FMOD_INIT_DSOUND_HRTFNONE;

    check_fmod(m_native->init(100, flags, 0), "FMOD::System::init");
}

system::~system()
{
    assert(m_native);
    m_native->release();
}

std::shared_ptr<sound> system::create_sound(pt::path const& path)
{
    assert(m_native);

    file_input_stream file_stream(path);
    data_stream_content file(file_stream);

    FMOD::Sound* native = 0;
    FMOD_CREATESOUNDEXINFO info;
    std::memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.length = static_cast<unsigned int>(file.size());

    check_fmod(m_native->createSound(
        reinterpret_cast<char *>(file.data()), FMOD_OPENMEMORY | FMOD_SOFTWARE, &info,
        &native), "FMOD::System::createSound");

    if (!native)
        throw fmod_error(FMOD_OK, "FMOD::System::createSound");

    return std::make_shared<sound>(shared_from_this(), native);
}

void system::update()
{
    check_fmod(m_native->update(), "FMOD::System::update");
}

//FMOD::Channel* system::play(shared_ptr<sound> const& sound)
//{
//    bool paused = true;
//    FMOD::Channel* channel = 0;
//    check_fmod(m_native->playSound(FMOD_CHANNEL_FREE, sound->native(), paused,
//        &channel), "FMOD::System::playSound");
//
//    if (!channel)
//        throw fmod_error(FMOD_OK, "FMOD::System::createSound");
//    return channel;
//}

FMOD::System* system::native()
{
    assert(m_native);
    return m_native;
}

} // namespace fmod_impl
