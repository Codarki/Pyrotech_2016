#ifndef CONTENT_FREE_VIEW_H
#define CONTENT_FREE_VIEW_H

#include "pt/d3d9/rendering/item.h"
#include "pt/d3d9/rendering/view.h"
#include "pt/ui/event_handler.h"
#include "pt/matrix.h"
#include "pt/ui/dragging_tool.h"
#include "pt/basic_value_source.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 {
    class text_source;
    template<typename T,int C>
    class formatted_text_source;

    namespace rendering {
        class projection_matrix;
        class srt_transformation;
        class simple_transformation;
        class transformation;
    }
}}

namespace content {

using namespace pt;

class free_view_controller : public ui::event_handler
{
public:
    free_view_controller();
    //parameter_collection const& parameters() const;

    // Maybe these two shouldnt be const functions.
    std::shared_ptr<d3d9::rendering::transformation> transformation() const;
    std::shared_ptr<d3d9::rendering::projection_matrix> projection() const;
	std::shared_ptr<d3d9::text_source const> info_text() const;

    void update(float time);

public: // interface required by ui::event_handler
    void handle_event(ui::event& event);

private: // helper functions
    void update(float heading, float pitch);

private: // data members
    //parameter_collection m_parameters;
    std::shared_ptr<d3d9::rendering::projection_matrix> m_projection;
    std::shared_ptr<d3d9::formatted_text_source<float, 7> > m_info_text;

    std::shared_ptr<basic_value_source<float> > m_fov;
    std::shared_ptr<basic_value_source<vector3f> > m_scale;
    std::shared_ptr<basic_value_source<vector3f> > m_view_position_in_world;
    std::shared_ptr<d3d9::rendering::simple_transformation> m_transformation;
    float m_pitch;
    float m_heading;
    float m_last_update_time;
    vector3f m_direction_in_world;
    vector3f m_side_vector_in_world;
    ui::dragging_tool m_dragging_tool;
};

} // namespace content

#endif
