#ifndef PT_D3D9_SHADERS_SIMPLE_SURFACE_SHADER_H
#define PT_D3D9_SHADERS_SIMPLE_SURFACE_SHADER_H

#include "surface_shader.h"
#include "stream_collection.h"
#include "pt/config_node.h"
#include "pt/path.h"

namespace pt { namespace d3d9 { namespace shaders {

/// Reads surface shader from a file.
class simple_surface_shader : public surface_shader
{
public:
    simple_surface_shader(pt::path const& path);

public: // interface required by surface_shader
    std::string const& pixel_shader() const final override;
    std::string const& pixel_shader_name() const final override;
    std::string const& vertex_shader() const final override;
    std::string const& vertex_shader_name() const final override;
    std::string const& name() const final override;
    stream_collection const& streams() const final override;
    config_node const& parameters() const final override;
    config_node const& sampler_states() const final override;
    bool is_additive_blended() const final override;
    bool is_transparent() const final override;
    bool is_alpha_tested() const final override;
    bool is_pixel_required_for_depth() const final override;
    int d3d_cullmode() const final override;

private: // type definitions
    struct vertex_shader_desc
    {
        void read(config_node const& config);
        std::string stream;
        std::string hlsl;
        std::string function_name;
    };
    struct pixel_shader_desc
    {
        void read(config_node const& config);
        std::string hlsl;
        std::string function_name;
    };

private: // data members
    pt::path m_path;
    std::string m_name;
    vertex_shader_desc m_vertex_shader;
    pixel_shader_desc m_pixel_shader;
    stream_collection m_streams;
    config_node m_parameters;
    config_node m_sampler_states;
    bool m_is_additive_blended;
    bool m_is_transparent;
    bool m_is_alpha_tested;
    bool m_is_pixel_required_for_depth;
    int m_d3d_cullmode;
};

}}} // namespace pt::d3d9::shaders

#endif
