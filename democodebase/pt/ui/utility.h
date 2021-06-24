#ifndef PT_UI_UTILITY_H
#define PT_UI_UTILITY_H

namespace pt { namespace ui {

/// Helper function to check type of \a event. \return \c true if \a event
/// is type of shared_ptr<T>. \note This is a dynamic_cast in disquise, so
/// avoid using when possible (design flaw). \relatesalso event
template<typename T>
bool is(shared_ptr<ui::event> const& event)
{
    if (shared_ptr<T> p
        = dynamic_pointer_cast<T>(event))
    {
        return true;
    }
    return false;
}

}} // namespace pt::ui

#endif
