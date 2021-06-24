#ifndef PT_UI_TABLE_H
#define PT_UI_TABLE_H

#include "bounded_element.h"
//#include "pt/std/vector.h"

namespace pt { namespace ui {

/// Abstract UI element with 2d bounds.
class table : public bounded_element
{
public: // type definitions
//    class row;
//    class column;
//
//public:
//    table(box2i const& bounds);
//
//    void set_column_count(size_t count);
//    size_t column_count() const;
//    void set_row_count(size_t count);
//    size_t row_count() const;
//
//    shared_ptr<element const> element_at(size_t row, size_t column) const;
//
//private: // ui::invalidatable virtual function implementations
//    void do_invalidate();
//
//private: // ui::bounded_element virtual function overrides
//    void on_set_bounds();
//
//private: // type definitions
//    typedef std::vector<shared_ptr<element> > row_container;
//    typedef std::vector<row_container> element_container;
//
//    struct column
//    {
//        int width;
//        std::vector<int> heights;
//    };
//
//    struct row
//    {
//        std::vector<int> widths;
//        float height;
//    };
//
//private: // data members
//    element_container m_elements;
};

}} // namespace pt::ui

#endif
