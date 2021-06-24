#include "demo_player.h"
#include "../application.h"
#include "scene_player.h"
#include "timeline_player.h"
#include "utility.h"
#include "sequences/post_processing_parameters.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/log.h"
#include "pt/parameter_node.h"
#include "pt/progress_report.h"
#include "pt/path.h"
#include "pt/exceptions/file_error.h"
#include "pt/d3d9/formatted_text_source.h"
#include "pt/d3d9/device_convenience.h"
#include "pt/d3d9/scoped_convenience.h"
#include "pt/d3d9/texture_2d_collection.h"
#include "pt/d3d9/dynamic_vertex_buffer.h"
#include "pt/d3d9/rendering/camera.h"
#include "pt/d3d9/rendering/camera_builder.h"
#include "pt/d3d9/rendering/item_name.h"
#include "pt/d3d9/rendering/projection_matrix.h"
#include "pt/d3d9/rendering/scene_builder.h"
#include "pt/d3d9/rendering/transformer.h"
#include "pt/d3d9/rendering/transformer_builder.h"
#include "pt/ui/close_event.h"
#include "pt/ui/common.h"
#include "pt/ui/draw_event.h"
#include "pt/ui/element_registry.h"

#include "pt/boost/filesystem/operations.h"

#include "user_abort.h"

#include "../spline_test.h"
#include "../plasma.h"
#include "pt/d3d9/rendering/scene.h"

#include "pt/file_output_stream.h"
#include "pt/file_openmode.h"
#include "pt/file_type.h"
#include "pt/path.h"

#include "pt/boost/lexical_cast.h"

namespace content {

namespace {
    std::shared_ptr<d3d9::rendering::item> get_free_camera(
        free_view_controller const& controller)
    {
        d3d9::rendering::transformer_builder transformer_builder(controller.transformation());
        auto shared_item = std::make_shared<d3d9::rendering::item>("free_view", transformer_builder.unique_transformer());

        std::vector<animatable_value<math::plane>> clipping_planes;

        d3d9::rendering::camera_builder builder(
            transformation_from(shared_item->transformer()), controller.projection(), clipping_planes, false);

        shared_item->set_camera(builder.unique_camera());
        return shared_item;
    }
    std::shared_ptr<d3d9::rendering::item> add_free_camera(
        free_view_controller const& controller,
		boost::shared_ptr<d3d9::rendering::scene> scene)
    {
        auto shared_item = get_free_camera(controller);
        scene->add_camera_item(shared_item);
        return shared_item;
    }

    //LARGE_INTEGER performance_frequency;

    //void query_performance_frequency()
    //{
    //	BOOL result = QueryPerformanceFrequency(&performance_frequency);
    //	if (!result)
    //		throw any_error("installed hardware does not support a high-resolution performance counter");
    //}

    //float get_time()
    //{
    //	LARGE_INTEGER performance_count;
    //	BOOL result = QueryPerformanceCounter(&performance_count);
    //	if (!result)
    //		throw any_error("QueryPerformanceCounter failed");

    //	float time = static_cast<float>(
    //		performance_count.QuadPart / static_cast<double>(performance_frequency.QuadPart));
    //	return time;
    //}

} // anonymous namespace

demo_player::demo_player(d3d9::graphics_device& device,
    command_line* commands, progress_report& progress,
    ui::invalidatable* invalidatable)
: m_device(device)
, m_commands(commands)
, m_info_display(device, vector2i(800, 600))
, m_finished(false)
, m_show_info_display(false)
, m_frame_count(0)
, m_use_free_view(false)
, m_current_view(0)
, m_paused(false)
, m_camera_index(0)
, m_global_start_time_ms(0)
, m_global_current_time_ms(0)
, m_invalidatable(invalidatable)
, m_sync_file_open(false)
, m_sync_gap_after(0.1f)
, m_sync_gap_before(0.1f)
, m_sync_min_value(0.0f)
, m_sync_max_value(1.0f)
, m_sync_wait_time(0.0f)
{
    //query_performance_frequency();

    std::shared_ptr<parameter_node> curves = parse_curve_parameters("data/curves/curves_test.xml");
    progress.advance();

    parse_shared_textures("data/shared_textures.xml", curves->children(), device);
    progress.advance();

    {
        auto audio_spectrum_segment_1_source = std::make_shared<basic_value_source<float>>(0.0f);
        auto audio_spectrum_segment_1_value = std::make_shared<animatable_value<float>>(audio_spectrum_segment_1_source);
        std::shared_ptr<parameter_node> audio_spectrum_segment_1_parameter(new parameter_node("audio_spectrum_segment_1", audio_spectrum_segment_1_value));
        curves->children().add(audio_spectrum_segment_1_parameter);
    }

    {
        auto audio_spectrum_segment_2_source = std::make_shared<basic_value_source<float>>(0.0f);
        auto audio_spectrum_segment_2_value = std::make_shared<animatable_value<float>>(audio_spectrum_segment_2_source);
        std::shared_ptr<parameter_node> audio_spectrum_segment_2_parameter(new parameter_node("audio_spectrum_segment_2", audio_spectrum_segment_2_value));
        curves->children().add(audio_spectrum_segment_2_parameter);
    }

    {
        auto audio_spectrum_segment_3_source = std::make_shared<basic_value_source<float>>(0.0f);
        auto audio_spectrum_segment_3_value = std::make_shared<animatable_value<float>>(audio_spectrum_segment_3_source);
        std::shared_ptr<parameter_node> audio_spectrum_segment_3_parameter(new parameter_node("audio_spectrum_segment_3", audio_spectrum_segment_3_value));
        curves->children().add(audio_spectrum_segment_3_parameter);
    }

    {
        auto audio_spectrum_segment_4_source = std::make_shared<basic_value_source<float>>(0.0f);
        auto audio_spectrum_segment_4_value = std::make_shared<animatable_value<float>>(audio_spectrum_segment_4_source);
        std::shared_ptr<parameter_node> audio_spectrum_segment_4_parameter(new parameter_node("audio_spectrum_segment_4", audio_spectrum_segment_4_value));
        curves->children().add(audio_spectrum_segment_4_parameter);
    }

    {
        auto audio_spectrum_vector4f_source = std::make_shared<basic_value_source<vector4f>>(vector4f());
        auto audio_spectrum_vector4f_value = std::make_shared<animatable_value<vector4f>>(audio_spectrum_vector4f_source);
        std::shared_ptr<parameter_node> audio_spectrum_vector4f_parameter(new parameter_node("audio_spectrum_vector4f", audio_spectrum_vector4f_value));
        curves->children().add(audio_spectrum_vector4f_parameter);
    }

    m_materials = std::make_unique<d3d9::rendering::material_cache>(curves->children());
    m_materials->load_materials("data/materials");
    progress.advance();

    m_post_processing_parameters.reset(
        new sequences::post_processing_parameters(
        get_post_processing_parameter_config(), curves));
    progress.advance();

    parameter_collection parameters
        = m_post_processing_parameters->parameter()->children();
    parameters.add(curves);

    //m_post_processing.reset(new d3d9::rendering::post_processing(
    //    device, post_processing_parameters));

    std::string timeline_name;
    if (m_commands->has("timeline"))
    {
        timeline_name = m_commands->value_of("timeline");
    }
    else
    {
        timeline_name = "data/timelines/unfault.xml";
    }

    /*
    if (m_commands->has("scene"))
    {
        std::string scene_name;
        m_commands->value_of("scene", scene_name);

        if (scene_name.empty())
        {
            throw any_error(
                "Use -scene=scene_file.p3d_scene to select a scene");
        }

        //if (!filesystem::exists(path(scene_name)))
        //    throw exceptions::file_error(path(scene_name), "file not found");

        bool serialize_only = m_commands->has("serialize_only");

        d3d9::rendering::scene_builder builder(
            device, scene_name, m_materials, &progress, serialize_only);

        shared_ptr<d3d9::rendering::scene> scene = builder.built_scene();
        m_scene_player.reset(new scene_player(device, scene));
        set_light_boxes(device, scene, m_materials);

        m_view = add_free_camera(m_free_view, scene);
        m_current_view = m_view.get();
    }
    else if (m_commands->has("timeline"))
    */
    {
        //std::string timeline_name;
        //m_commands->value_of("timeline", timeline_name);

        if (timeline_name.empty())
        {
            throw any_error(
                "Use -timeline=timeline_file.xml to select a timeline");
        }

        //if (!filesystem::exists(path(timeline_name)))
        //{
        //    throw exceptions::file_error(
        //        path(timeline_name), "file not found");
        //}

        bool serialize_only = m_commands->has("serialize_only");

        m_timeline_player.reset(new timeline_player(
            device, timeline_name, *m_materials,
            parameters, &progress, serialize_only));

        m_view = get_free_camera(m_free_view);
        m_current_view = m_view.get();
    }

    if (m_commands->has("sync_params"))
    {
        std::string sync_params = m_commands->value_of("sync_params");

        const int numParams = 5;
        size_t found[numParams-1];
        std::string l_sync_params[numParams];

        for (int i=0;i<numParams-1;i++) {
            found[i] = sync_params.find(";",i > 0 ? found[i-1]+1 : 0);
        }
        /*for (int i=0;i<3;i++) {
            if (i==0) {
                l_sync_params[i] = sync_params.substr(0,found[i]-1);
            }
            else if (i>0 && i < 2)
                l_sync_params[i] = sync_params.substr(found[i]+1,found[i+1]-found[i]);
            else
                l_sync_params[i] = sync_params.substr(found[i]+1);
        }*/
        l_sync_params[0] = sync_params.substr(0,found[0]);
        l_sync_params[1] = sync_params.substr(found[0]+1,found[1]-found[0]-1);
        l_sync_params[2] = sync_params.substr(found[1]+1,found[2]-found[1]-1);
        l_sync_params[3] = sync_params.substr(found[2]+1,found[3]-found[2]-1);
        l_sync_params[4] = sync_params.substr(found[3]+1);
        
        for (int i=0;i<numParams;i++) {
            log(l_sync_params[i]);
        }

        /*const std::string &l_sync_param_1 = (std::string const &)l_sync_params[0];
        const std::string &l_sync_param_2 = (std::string const &)l_sync_params[1];
        const std::string &l_sync_param_3 = (std::string const &)l_sync_params[2];
        const std::string &l_sync_param_4 = (std::string const &)l_sync_params[3];
        */
        m_sync_gap_before = lexical_cast<float>(l_sync_params[0]);
        m_sync_gap_after = lexical_cast<float>(l_sync_params[1]);
        m_sync_wait_time = lexical_cast<float>(l_sync_params[2]);
        m_sync_min_value = lexical_cast<float>(l_sync_params[3]);
        m_sync_max_value = lexical_cast<float>(l_sync_params[4]);
        
    }

    //m_current_view = &m_free_view;

    // Default to free view.
    if (m_scene_player)
        m_scene_player->set_view(m_current_view);

    // Setup info display.
    m_info_text.reset(new d3d9::formatted_text_source<float,3>());
    m_info_text->set(boost::format("Time: %1% Frame: %2% Fps: %3%"));
    m_info_text->set(0, 0);
    m_info_text->set(1, 0);
    m_info_text->set(2, 0);

    m_info_display.add(m_info_text);
    m_info_display.add(m_free_view.info_text());

	typedef std::vector<std::shared_ptr<d3d9::text_source> > source_container;
    for (source_container::const_iterator i
        = m_post_processing_parameters->text_sources().begin();
        i != m_post_processing_parameters->text_sources().end(); ++i)
    {
        m_info_display.add(*i);
    }

    //m_spline_test.reset(new spline_test(device));
    //m_plasma.reset(new plasma(device));

    //// print parameters
    //{
    //    pt::file_openmode mode = pt::openmode::append;
    //    file_output_stream file("parameter_hierarchy.txt", file_type(true), mode);
    //    write(file, hierarchy_as_readable_text(*m_post_processing_parameters->parameter()).c_str());
    //    write(file, hierarchy_as_readable_text(*curves).c_str());
    //    write(file, m_materials.parameters_as_readable_text().c_str());
    //}
}

demo_player::~demo_player()
{
    log("Destructing demo_player.");
    clear_global_program_cache();

    d3d9::clear_default_texture_2d_collection();
    d3d9::clear_default_depth_stencil();
    //d3d9::dynamic_vertex_buffer::clean_up();

    //ui::element_registry::instance().clear();
}

void demo_player::update()
{
    size_t global_new_time_ms = ::GetTickCount();

    if (m_global_start_time_ms == 0)
        m_global_start_time_ms = global_new_time_ms;

    if (m_paused)
    {
        assert(global_new_time_ms >= m_global_current_time_ms);
        m_global_start_time_ms += global_new_time_ms - m_global_current_time_ms;
    }

    m_global_current_time_ms = global_new_time_ms;

    assert(global_new_time_ms >= m_global_start_time_ms);
    m_local_current_time_s = (global_new_time_ms - m_global_start_time_ms) / 1000.0f;

    m_post_processing_parameters->set_current_time(m_local_current_time_s);
    //m_plasma->update(m_time);

    try
    {
        m_free_view.update(m_local_current_time_s);

        if (m_scene_player)
        {
            if (m_use_free_view)
                m_scene_player->set_view(m_current_view);

            //m_scene_player->set_view_at(m_camera_index);
            m_scene_player->update(m_local_current_time_s);
        }
        else if (m_timeline_player)
        {
            //if (m_use_free_view)
            //    m_timeline_player->set_view(m_current_view);
            //else
            //    m_timeline_player->set_view(0);

            ////m_timeline_player->set_view(0);
            //m_timeline_player->update(m_local_current_time_s);
        }
        //m_post_processing->update(m_time);

        m_info_text->set(0, static_cast<float>(m_local_current_time_s));
        m_info_text->set(1, static_cast<float>(m_frame_count));
        m_info_text->set(2, static_cast<float>(m_frame_count / m_local_current_time_s));
    }
    catch(content::user_abort&)
    {
        log("User aborted.");
        m_finished = true;
        return;
    }
    catch(std::exception& e)
    {
        log(std::string("demo_player::update exception ") + e.what());
        m_finished = true;
        throw;
    }
}

bool demo_player::finished() const
{
    return m_finished;
}

void demo_player::warm_up()
{
}

void demo_player::set_resolution(vector2i const& resolution)
{
    //D3DFORMAT format = D3DFMT_D24X8;
    //D3DMULTISAMPLE_TYPE multi_sample = D3DMULTISAMPLE_NONE;
    //DWORD multi_sample_quality = 0;

    //m_depth_stencil = d3d9::create_depth_stencil_surface(device(),
    //    resolution[0], resolution[1], format, multi_sample,
    //    multi_sample_quality, true);

    m_free_view.projection()->set_resolution(resolution);
}

void demo_player::handle_event(ui::event& event)
{
    try
    {
        m_free_view.handle_event(event);
        dispatch(event);
    }
    catch (std::exception& e)
    {
        log(std::string("demo_player::handle_event exception, aborting: ") + e.what());
        m_finished = true;
        throw;
    }
    catch(...)
    {
        log("demo_player::handle_event exception, aborting.");
        m_finished = true;
        throw;
    }
}

void demo_player::handle_close(ui::close_event& event)
{
    event.set_ok_cancel(true);
    m_finished = true;
}

void demo_player::handle_draw(ui::draw_event&)
{
    update();

    if (m_use_free_view)
        m_timeline_player->set_view(m_current_view);
    else
        m_timeline_player->set_view(nullptr);

    //m_timeline_player->update(m_local_current_time_s);

    {
        //d3d9::rendering::scoped_post_processing post_process(*m_post_processing);
        //d3d9::set_depth_stencil_surface(device(), m_depth_stencil);

        //if (m_scene_player)
        //{
        //    // Hack for scene player.
        //    DWORD flags = D3DCLEAR_TARGET;

        //    //if (m_depth_stencil)
        //    //    flags |= D3DCLEAR_ZBUFFER;

        //    //vector4f clear_color(0, 0, 0.25f, 0);
        //    vector4f clear_color(0, 0, 0, 1.0f);
        //    d3d9::clear(m_device, flags, clear_color, 1.0f, 0);

        //    d3d9::scoped_render_state culling(m_device, D3DRS_CULLMODE, D3DCULL_CCW);
        //    d3d9::scoped_render_state zenable(m_device, D3DRS_ZENABLE, TRUE);

        //    m_scene_player->draw();
        //}
        //else if (m_timeline_player)
        {
            m_timeline_player->draw(m_local_current_time_s);
        }
        //m_plasma->draw();
    }
    //m_post_processing->execute();

    //m_spline_test->draw();

    if (m_show_info_display)
        m_info_display.draw();

    if (!m_paused)
        ++m_frame_count;

    //m_invalidatable->invalidate();
}

void demo_player::handle_key_down(ui::key_down_event& event)
{
    if (event.key_code() == VK_ESCAPE)
        m_finished = true;

    if (event.key_code() == 'C')
        m_use_free_view = !m_use_free_view;

    if (event.key_code() == 'I' || event.key_code() == VK_F1)
        m_show_info_display = !m_show_info_display;

    if (event.key_code() == 'R')
    {
        m_global_start_time_ms = ::GetTickCount();
        m_global_current_time_ms = m_global_start_time_ms;
        m_local_current_time_s = 0.0f;
        m_frame_count = 0;
    }

    if (event.key_code() == VK_F2)
    {
        pt::file_openmode mode = pt::openmode::append;
        file_output_stream file("parameter_hierarchy.txt", file_type(true), mode);
        write(file, hierarchy_as_readable_text(*m_post_processing_parameters->parameter()).c_str());
        //write(file, hierarchy_as_readable_text(*curves).c_str());
        write(file, m_materials->parameters_as_readable_text().c_str());
    }

    if (event.key_code() == VK_SPACE)
        m_paused = !m_paused;

    if (event.key_code() == VK_OEM_COMMA)
    {
        size_t dt_ms = 0;

        if (ui::is_key_down(VK_SHIFT))
            dt_ms += 1000 / 30;
        else if (ui::is_key_down(VK_CONTROL))
            dt_ms += 3000;
        else
            dt_ms += 1000;

        m_global_start_time_ms += dt_ms;
        m_global_current_time_ms -= dt_ms;

        if (m_global_current_time_ms < m_global_start_time_ms)
            m_global_current_time_ms = m_global_start_time_ms;
    }
    if (event.key_code() == VK_OEM_PERIOD)
    {
        size_t dt_ms = 0;

        if (ui::is_key_down(VK_SHIFT))
            dt_ms += 1000 / 30;
        else if (ui::is_key_down(VK_CONTROL))
            dt_ms += 3000;
        else
            dt_ms += 1000;

        size_t real_delta = m_global_start_time_ms >= dt_ms ? dt_ms : 0;

        m_global_start_time_ms -= real_delta;
        //m_global_current_time_ms += real_delta;
    }

    m_post_processing_parameters->handle_key_down(event);

    if (event.key_code() == VK_RETURN) {
        // Truncate if first call, otherwise append.
        pt::file_openmode mode = m_sync_file_open
            ? pt::openmode::append : pt::openmode::truncate;

        m_sync_file_open = true;

        float cur_time = m_local_current_time_s;
        float cur_time_min_1 = m_local_current_time_s - m_sync_gap_before;
        float cur_time_wait_time = m_local_current_time_s + m_sync_wait_time;
        float cur_time_add_1 = cur_time_wait_time +	m_sync_gap_after;
        

        std::string outputString("");
        std::string outputString_1("");
        std::string outputString_2("");
        std::string outputString_wait_time("");

        //Create sync tag cur time
        //outputString.append("<key time = ");
        outputString.append(boost::lexical_cast<std::string>(cur_time));
        //outputString.append(" value = ");
        outputString.append(";");
        outputString.append(boost::lexical_cast<std::string>(m_sync_max_value));
        //outputString.append("/>");
        outputString.append("\r\n");

        //Create sync tag cur time - 1
        //outputString_1.append("<key time = ");
        outputString_1.append(boost::lexical_cast<std::string>(cur_time_min_1));
        //outputString_1.append(" value = ");
        outputString_1.append(";");
        outputString_1.append(boost::lexical_cast<std::string>(m_sync_min_value));
        //outputString_1.append("/>");
        outputString_1.append("\r\n");

        //Create sync tag cur time + 1
        //outputString_2.append("<key time = ");
        outputString_2.append(boost::lexical_cast<std::string>(cur_time_add_1));
        //outputString_2.append(" value = ");
        outputString_2.append(";");
        outputString_2.append(boost::lexical_cast<std::string>(m_sync_min_value));
        //outputString_2.append("/>");
        outputString_2.append("\r\n");

        if (m_sync_wait_time > 0) {
            //Create sync wait time;
            //outputString.append("<key time = ");
            outputString_wait_time.append(boost::lexical_cast<std::string>(cur_time_wait_time));
            //outputString.append(" value = ");
            outputString_wait_time.append(";");
            outputString_wait_time.append(boost::lexical_cast<std::string>(m_sync_max_value));
            //outputString.append("/>");
            outputString_wait_time.append("\r\n");
        }

        file_output_stream file("sync.txt", file_type(true), mode);
        write(file, outputString_1.c_str());
        write(file, outputString.c_str());
        write(file, outputString_wait_time.c_str());
        write(file, outputString_2.c_str());
    }
    //if (event.key_code() == 'P')
    //    m_post_processing_parameters->clear_overrides();
}

} // namespace content
