#pragma once
#ifndef CONTENT_SEQUENCES_COMPOSITE_SEQUENCE_H
#define CONTENT_SEQUENCES_COMPOSITE_SEQUENCE_H

#include "pt/d3d9/rendering/camera_fwd.h"
#include "pt/d3d9/texture_2d_fwd.h"
#include "pt/string_wrapper_fwd.h"
#include "pt/std/memory.h"

namespace content { namespace sequences {

class composite_sequence_collection;
class rendertarget;

/// Abstract base class for composition of sequences.
class composite_sequence
{
public:
    virtual ~composite_sequence();

    /// Returns the name of this sequence
    virtual pt::string_wrapper const& name() const = 0;

    /// Draws this sequence to given target.
    virtual std::shared_ptr<pt::d3d9::texture_2d> draw(
        sequences::rendertarget& target, float time) = 0;

    /// Returns the start time of this sequence
    virtual float start_time() const = 0;

    /// Returns the end time of this sequence
    virtual float end_time() const = 0;

    virtual void set_override_view(pt::d3d9::rendering::item* view) = 0;

    virtual void link_inputs(composite_sequence_collection const& sequences) = 0;
};

}} // namespace content::sequences

#endif
