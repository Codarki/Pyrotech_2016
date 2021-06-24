#ifndef PT_D3D9_RENDERING_DETAIL_SETTER_CONTAINER_H
#define PT_D3D9_RENDERING_DETAIL_SETTER_CONTAINER_H

#include "pt/d3d9/constant_buffer_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/map.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class variable_setter;

namespace detail {
    /// Constant setter containers mapped to a constant buffer.
    class setter_container
    {
    public:
        setter_container();
        ~setter_container();

        /// Adds \a setter for \a buffer.
        void add(constant_buffer const* buffer,
            std::unique_ptr<variable_setter>&& setter);

        /// Updates constants in \a buffer.
        void update_target(constant_buffer const* buffer, float time) const;

        /// Returns \c true if this container has any setters for \a buffer.
        bool has_any_setter(constant_buffer const* buffer) const;

    private: // type definitions
        typedef std::vector<std::unique_ptr<variable_setter> > setters;

    private: // helper functions
        bool has(constant_buffer const*, variable_setter const& setter) const;

    private: // data members
        std::map<constant_buffer const*, setters> m_setters;
    };
} // namespace detail

}}} // namespace pt::d3d9::rendering

#endif
