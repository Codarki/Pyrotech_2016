#include "basic_composite_sequence.h"
#include "composite_sequence_collection.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/format.h"
#include "pt/string_wrapper.h"
#include "pt/std/algorithm.h"
#include "../utility.h"
#include "pt/log.h"

namespace content { namespace sequences {

using namespace pt;

// basic_composite_sequence::input

basic_composite_sequence::input::input()
: depth(0)
, start_time_offset(0.0f)
, duration(-std::numeric_limits<float>::max())
{
}

bool basic_composite_sequence::input::is_infinite()
{
    return duration == -std::numeric_limits<float>::max();
}

bool basic_composite_sequence::input::operator()(
    input const* a, input const* b) const
{
    return a->depth > b->depth;
}

// basic_composite_sequence

basic_composite_sequence::basic_composite_sequence(config_node const& config)
: m_start_time(0.0f)
, m_duration(-std::numeric_limits<float>::max())
, m_current_local_time(-std::numeric_limits<float>::max())
, m_is_infinite(true)
{
    {
        std::string name;
        config.parse_child("name", name);
        m_name = std::move(name);
    }

    config.parse_optional_child("start_time", m_start_time);
    config.parse_optional_child("duration", m_duration);

    if (m_duration != -std::numeric_limits<float>::max())
        m_is_infinite = false;

    for (config_node::const_iterator i = config.begin(); i != config.end();
        ++i)
    {
        if (i->key() == "input")
        {
            input input;

            {
                std::string name;
                i->parse_child("sequence", name);
                input.name = name;
            }
            i->parse_optional_child("depth", input.depth);
            i->parse_optional_child("time", input.start_time_offset);
            i->parse_optional_child("duration", input.duration);


            m_inputs.push_back(input);
        }
    }
}

basic_composite_sequence::basic_composite_sequence(config_node const& config,
    parameter_collection const &parameters)
: m_start_time(0.0f)
, m_duration(-std::numeric_limits<float>::max())
, m_current_local_time(-std::numeric_limits<float>::max())
, m_is_infinite(true)
{
    {
        std::string name;
        config.parse_child("name", name);
        m_name = std::move(name);
    }

    config.parse_optional_child("start_time", m_start_time);
    config.parse_optional_child("duration", m_duration);

    if (m_duration != -std::numeric_limits<float>::max())
        m_is_infinite = false;

    // Optional time curve to distort sequence's local time.
    auto time_curve_config_iter = config.find_child("time_curve");
    if (time_curve_config_iter != config.end())
    {
        std::shared_ptr<parameter_node> curves = *parameters.child("curves");
        m_time_curve = create_curve_value<float>(
            *time_curve_config_iter, curves);
    }
    else
    {
        log(std::string("Basic composite ") + m_name + std::string(" has no time_curve attribute"));
    }


    for (config_node::const_iterator i = config.begin(); i != config.end();
        ++i)
    {
        if (i->key() == "input")
        {
            input input;

            {
                std::string name;
                i->parse_child("sequence", name);
                input.name = name;
            }
            i->parse_optional_child("depth", input.depth);
            i->parse_optional_child("time", input.start_time_offset);
            i->parse_optional_child("duration", input.duration);

            m_inputs.push_back(input);
        }
    }
}

basic_composite_sequence::basic_composite_sequence(std::string const& name)
: m_start_time(0.0f)
, m_duration(-std::numeric_limits<float>::max())
, m_current_local_time(-std::numeric_limits<float>::max())
, m_is_infinite(true)
, m_name(name)
{
}

basic_composite_sequence::const_iterator basic_composite_sequence::begin()
    const
{
    return m_inputs.begin();
}

basic_composite_sequence::const_iterator basic_composite_sequence::end() const
{
    return m_inputs.end();
}

void basic_composite_sequence::add_child(input child)
{
    m_inputs.push_back(child);
}

void basic_composite_sequence::set_end_time(float time)
{
    // TODO: Should the time curve affect the duration?

    float l_time = get_time_internal(time);

    if (l_time < m_start_time)
    {
        throw any_error(
            format("'%1%': End time can't be greater than start time.") % m_name);
    }

    m_duration = l_time - m_start_time;
    m_is_infinite = false;

    if (l_time == -std::numeric_limits<float>::max())
    {
        m_duration = -std::numeric_limits<float>::max();
        m_is_infinite = true;
    }
}

string_wrapper const& basic_composite_sequence::name() const
{
    return m_name;
}

std::shared_ptr<d3d9::texture_2d> basic_composite_sequence::draw(
    sequences::rendertarget& target, float time)
{
    float l_time = get_time_internal(time);
    if (l_time < m_start_time)
        return nullptr;

    float local_time = l_time - m_start_time;

    if (!m_is_infinite && local_time > m_duration)
        return nullptr;

    m_current_local_time = local_time;

    get_active_sorted_inputs();

    std::shared_ptr<d3d9::texture_2d> result;

    for (std::vector<input*>::const_iterator i = m_active_inputs.begin();
        i != m_active_inputs.end(); ++i)
    {
        float transformed_time = local_time - (*i)->start_time_offset;
        std::shared_ptr<d3d9::texture_2d> new_result
            = (*i)->sequence->draw(target, transformed_time);
        
        if (new_result)
        {
            //if (result)
            //    throw any_error(m_name + ": Multiple inputs returning "
            //    " texture, only single result supported from composite "
            //    " sequence.");

            result = new_result;
        }
    }

    return result;
}

float basic_composite_sequence::start_time() const
{
    return m_start_time;
}

float basic_composite_sequence::end_time() const
{
    if (!m_is_infinite)
        return m_start_time + m_duration;
    else
        return std::numeric_limits<float>::max();
}

void basic_composite_sequence::set_override_view(d3d9::rendering::item* view)
{
    for (std::vector<input*>::iterator i = m_active_inputs.begin();
        i != m_active_inputs.end(); ++i)
    {
        (*i)->sequence->set_override_view(view);
    }
}

void basic_composite_sequence::link_inputs(composite_sequence_collection const& sequences)
{
    typedef std::vector<std::shared_ptr<composite_sequence> > sequence_vector;

    for (container::iterator i = m_inputs.begin(); i != m_inputs.end();
        ++i)
    {
        if (i->sequence)
        {
            // input added via add_child().
            continue;
        }

        bool found = false;

        for (sequence_vector::const_iterator j = sequences.get().begin();
            j != sequences.get().end(); ++j)
        {
            if (i->name == (*j)->name())
            {
                found = true;
                i->sequence = *j;
                break;
            }
        }

        if (!found)
        {
            throw any_error(m_name + ": input sequence \""
                + i->name + "\" not found");
        }
    }
}

void basic_composite_sequence::get_active_sorted_inputs()
{
    m_active_inputs.clear();
    for (container::iterator i = m_inputs.begin(); i != m_inputs.end();
        ++i)
    {
        std::shared_ptr<composite_sequence>& input_sequence = i->sequence;

        float transformed_time = m_current_local_time - i->start_time_offset;

        if (!i->is_infinite() && transformed_time > i->duration)
            continue;

        if (transformed_time >= input_sequence->start_time()
            && transformed_time < input_sequence->end_time())
        {
            m_active_inputs.push_back(&*i);
        }
    }

    std::sort(m_active_inputs.begin(), m_active_inputs.end(), input());
}

float basic_composite_sequence::get_time_internal(float time)
{
    if (m_time_curve == nullptr)
        return time;

    auto distorted_time = m_time_curve->value_at(time);

    // Clamp start time.
    if (distorted_time < m_start_time)
        return m_start_time;

    // TODO: Is there a way to clamp end time? Should we do it?

    return distorted_time;
}


}} // namespace content::sequences
