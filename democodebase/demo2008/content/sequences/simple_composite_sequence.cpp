#include "simple_composite_sequence.h"

namespace content { namespace sequences {

// simple_composite_sequence

simple_composite_sequence::simple_composite_sequence(
	config_node const& config)
: m_basic(config)
{
}

simple_composite_sequence::simple_composite_sequence(std::string const& name)
: m_basic(name)
{
}

void simple_composite_sequence::add_child(
    std::shared_ptr<composite_sequence> sequence)
{
    add_child(sequence, sequence->name());
}

void simple_composite_sequence::add_child(
    std::shared_ptr<composite_sequence> sequence, string_wrapper const& name)
{
    basic_composite_sequence::input child;
    child.name = name;
    child.depth = 0;
    child.sequence = sequence;
    child.start_time_offset = 0.0f;

    m_basic.add_child(child);
}

void simple_composite_sequence::set_end_time(float time)
{
    m_basic.set_end_time(time);
}

string_wrapper const& simple_composite_sequence::name() const
{
    return m_basic.name();
}

std::shared_ptr<d3d9::texture_2d> simple_composite_sequence::draw(
    rendertarget& target, float time)
{
    //scoped_optional_rendertarget scoped_target(target);
	std::shared_ptr<d3d9::texture_2d> result = m_basic.draw(target, time);

    return on_draw(target, time, result);
}

float simple_composite_sequence::start_time() const
{
    return m_basic.start_time();
}

float simple_composite_sequence::end_time() const
{
    return m_basic.end_time();
}

void simple_composite_sequence::set_override_view(
    d3d9::rendering::item* camera)
{
    m_basic.set_override_view(camera);
}

void simple_composite_sequence::link_inputs(composite_sequence_collection const& sequences)
{
    m_basic.link_inputs(sequences);
}

std::shared_ptr<d3d9::texture_2d> simple_composite_sequence::on_draw(
    rendertarget&, float, std::shared_ptr<d3d9::texture_2d>)
{
    return nullptr;
}

}} // namespace content::sequences
