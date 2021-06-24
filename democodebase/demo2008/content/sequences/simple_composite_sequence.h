#pragma once
#ifndef CONTENT_SEQUENCES_SIMPLE_COMPOSITE_SEQUENCE_H
#define CONTENT_SEQUENCES_SIMPLE_COMPOSITE_SEQUENCE_H

#include "basic_composite_sequence.h"

namespace content { namespace sequences {

using namespace pt;

/// Simple implementation for composition of sequences.
/// Designed to be a base class, providing hook functions.
class simple_composite_sequence : public composite_sequence
{
public:
    void add_child(std::shared_ptr<composite_sequence> sequence);
    void add_child(std::shared_ptr<composite_sequence> sequence,
        pt::string_wrapper const& name);
    void set_end_time(float time);

public: // interface required by composite_sequence
    pt::string_wrapper const& name() const;
    std::shared_ptr<pt::d3d9::texture_2d> draw(rendertarget& target, float time);
    float start_time() const;
    float end_time() const;
    void set_override_view(pt::d3d9::rendering::item* view);
    void link_inputs(composite_sequence_collection const& sequences);

protected:
	simple_composite_sequence(pt::config_node const& config);
    simple_composite_sequence(std::string const& name);

private:
    virtual std::shared_ptr<pt::d3d9::texture_2d> on_draw(
        rendertarget& target,
        float time,
        std::shared_ptr<pt::d3d9::texture_2d> child_result);

private: // data members
    basic_composite_sequence m_basic;
};

}} // namespace content::sequences

#endif
