#pragma once
#ifndef CONTENT_SEQUENCES_BASIC_COMPOSITE_SEQUENCE_H
#define CONTENT_SEQUENCES_BASIC_COMPOSITE_SEQUENCE_H

#include "composite_sequence.h"
#include "pt/string_wrapper.h"
#include "pt/config_node_fwd.h"
#include "pt/std/vector.h"
#include "pt/parameter_collection.h"
#include "pt/animatable_value.h"

namespace content { namespace sequences {

/// Basic implementation of composite sequence.
/// Final. Do not inherit. See simple_composite_sequence for inheriting
/// functionality.
class basic_composite_sequence : public composite_sequence
{
public: // type definitions
    struct input
    {
        input();
        pt::string_wrapper name;
        int depth;
        std::shared_ptr<composite_sequence> sequence;
        float start_time_offset;
        float duration;

        bool is_infinite();
        bool operator()(basic_composite_sequence::input const* a,
            basic_composite_sequence::input const* b) const;
    };
    typedef std::vector<input> container;
    typedef container::const_iterator const_iterator;

public:
    basic_composite_sequence(pt::config_node const& config);
    basic_composite_sequence(pt::config_node const& config, pt::parameter_collection const& parameters);
    basic_composite_sequence(std::string const& name);

    const_iterator begin() const;
    const_iterator end() const;

    void add_child(input child);
    void set_end_time(float time);

public: // interface required by composite_sequence
    pt::string_wrapper const& name() const;
    std::shared_ptr<pt::d3d9::texture_2d> draw(rendertarget& target, float time);
    float start_time() const;
    float end_time() const;
    void set_override_view(pt::d3d9::rendering::item* view);
    void link_inputs(composite_sequence_collection const& sequences);

private:
    void get_active_sorted_inputs();
    float get_time_internal(float time);

private: // data members
    pt::string_wrapper m_name;
    float m_start_time;
    float m_duration;
    float m_current_local_time;
    bool m_is_infinite;
    std::shared_ptr<pt::animatable_value<float> const> m_time_curve;

    container m_inputs;
    std::vector<input*> m_active_inputs;

};

}} // namespace content::sequences

#endif
