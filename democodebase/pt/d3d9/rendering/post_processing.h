#ifndef PT_D3D9_RENDERING_POST_PROCESSING_H
#define PT_D3D9_RENDERING_POST_PROCESSING_H

#include "pt/boost/noncopyable.h"
#include "../texture_2d.h"
#include "pt/animatable_value.h"
#include "pt/std/memory.h"

namespace pt {
class parameter_collection;
}

namespace pt { namespace d3d9 { namespace rendering {

class bloom;
class blur;
class bright_pass_filter;
class bright_pass_filter2;
class tone_mapping;
class scale;
class noise;
class dof;
class light_scattering;

class post_processing : public device_reference
{
public:
    post_processing(graphics_device& device,
        parameter_collection const& parameters);

    void update(float time);
    void execute();

    void add_adapted_luminance(float delta);

    std::shared_ptr<texture_2d> input_color();

private: // data members
    com_ptr<IDirect3DSurface9> m_depth_stencil;
    std::shared_ptr<texture_2d> m_render_target_texture;

    std::shared_ptr<noise> m_noise;
    std::shared_ptr<bloom> m_bloom;
    std::shared_ptr<blur> m_blur;
	std::shared_ptr<blur> m_fullscreen_blur;
    std::shared_ptr<bright_pass_filter> m_bright_pass;
    //shared_ptr<bright_pass_filter2> m_bright_pass2;
    std::shared_ptr<tone_mapping> m_tone_mapping;
    std::shared_ptr<scale> m_scale;

    std::shared_ptr<scale> m_scale_to_bloom_source;
    std::shared_ptr<scale> m_scale_to_bloom_source2;
    std::shared_ptr<dof> m_dof;
	std::shared_ptr<light_scattering> m_light_scattering;

    std::shared_ptr<animatable_value<float> const> m_adapted_luminance;

    float m_current_adapted_luminance;
};

class scoped_post_processing : boost::noncopyable
{
public:
    scoped_post_processing(post_processing& post_processing);
    ~scoped_post_processing();

private:
    post_processing& m_post_processing;
    com_ptr<IDirect3DSurface9> m_old;
};

// free functions
void fill_fullscreen_quad(graphics_device& device);
void fill_quad(graphics_device& device, vector2f min, vector2f max);
void fill_quad(graphics_device& device, vector2f min, vector2f max, vector2f rotCenter, float rotation);

/// 2 parameter Gaussian distrubution using the given standard deviation \a rho
float gaussian_distribution(float x, float y, float rho);

std::string const& pass_through_vertex_shader_hlsl_with_texcoord();

}}} // namespace pt::d3d9::rendering

#endif
