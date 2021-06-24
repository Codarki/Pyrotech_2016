#pragma once
#ifndef PT_UI_COMMON_H
#define PT_UI_COMMON_H

namespace pt { namespace ui {

/// \return \c true if key \a key_code is pressed down.
bool is_key_down(int key_code);

/// \return \c true if key \a key_code is not pressed down.
bool is_key_up(int key_code);

}} // namespace pt::ui

#endif
