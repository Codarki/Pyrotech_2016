#include "post_processing_parameters_base.h"
#include "pt/parameter_node.h"
#include "pt/parameter_collection.h"
#include "pt/string_wrapper.h"

namespace content { namespace sequences {

// post_processing_parameters_base

post_processing_parameters_base::post_processing_parameters_base(
    std::string const& name)
: m_node(new parameter_node(name))
, m_time(0.0f)
{
}

post_processing_parameters_base::~post_processing_parameters_base()
{
}

std::shared_ptr<parameter_node const>
    post_processing_parameters_base::parameter() const
{
    return m_node;
}

std::shared_ptr<parameter_node> post_processing_parameters_base::parameter()
{
    return m_node;
}

void post_processing_parameters_base::add(std::shared_ptr<parameter_node> node)
{
    m_node->children().add(node);
}

void post_processing_parameters_base::set_current_time(float time)
{
    m_time = time;
    on_set_current_time(time);
}

void post_processing_parameters_base::handle_key_down(
    ui::key_down_event& event)
{
    on_handle_key_down(event);
}

void post_processing_parameters_base::clear_overrides()
{
    on_clear_overrides();
}

float post_processing_parameters_base::time() const
{
    return m_time;
}

void post_processing_parameters_base::on_set_current_time(float)
{
}

void post_processing_parameters_base::on_handle_key_down(
    ui::key_down_event&)
{
}

void post_processing_parameters_base::on_clear_overrides()
{
}

std::vector<std::shared_ptr<d3d9::text_source> > const&
    post_processing_parameters_base::text_sources() const
{
    return m_text_sources;
}

void post_processing_parameters_base::add_text_source(
    std::shared_ptr<d3d9::text_source> source)
{
    m_text_sources.push_back(source);
}

void post_processing_parameters_base::add_text_sources(
    std::vector<std::shared_ptr<d3d9::text_source> > const& sources)
{
    m_text_sources.insert(m_text_sources.end(), sources.begin(),
        sources.end());
}

}} // namespace content::sequences
