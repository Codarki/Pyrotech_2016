#include "common.h"
#include "pt/win32.h"

namespace pt { namespace ui {

bool is_key_down(int key_code)
{
    SHORT state = GetKeyState(key_code);
    return (state & 0x8000) != 0;
}

bool is_key_up(int key_code)
{
    return !is_key_down(key_code);
}

}} // namespace pt::ui
