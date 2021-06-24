#ifndef CONTENT_SEQUENCES_POST_PROCESSING_PARAMETERS_BASE_H
#define CONTENT_SEQUENCES_POST_PROCESSING_PARAMETERS_BASE_H

#include "pt/config_node_fwd.h"
#include "pt/parameter_node_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"
#include "pt/std/string.h"

namespace pt { namespace d3d9 {
    class text_source;
}}

namespace pt { namespace ui {
    class key_down_event;
}}

namespace content { namespace sequences {

using namespace pt;

class post_processing_parameters_base
{
public:
    post_processing_parameters_base(std::string const& name);
    virtual ~post_processing_parameters_base();

    std::shared_ptr<parameter_node const> parameter() const;
    std::shared_ptr<parameter_node> parameter();
    void add(std::shared_ptr<parameter_node> node);

    void set_current_time(float time);
    float time() const;
    void handle_key_down(ui::key_down_event& event);
    void clear_overrides();

	std::vector<std::shared_ptr<d3d9::text_source> > const& text_sources() const;
    void add_text_source(std::shared_ptr<d3d9::text_source> source);
    void add_text_sources(
        std::vector<std::shared_ptr<d3d9::text_source> > const& sources);

protected: // hook functions
    virtual void on_set_current_time(float time);
    virtual void on_handle_key_down(ui::key_down_event& event);
    virtual void on_clear_overrides();

private: // data members
    std::shared_ptr<parameter_node> m_node;
    std::vector<std::shared_ptr<d3d9::text_source> > m_text_sources;
    float m_time;
};

}} // namespace content::sequences

#endif
