#ifndef PT_D3D9_SHADERS_POINT_LIGHT_SHADER_H
#define PT_D3D9_SHADERS_POINT_LIGHT_SHADER_H

#include "light_shader.h"
#include "pt/config_node.h"
#include "pt/path.h"
#include "pt/std/memory.h"

namespace pt { namespace d3d9 { namespace shaders {

class basic_light_shader sealed : public light_shader
{
public:
    basic_light_shader(
        std::string const& name,
        std::string const& pixel_hlsl,
        config_node parameters,
        config_node sampler_states);

public: // interface required by light_shader
    std::string const& name() const override;
    std::string const& pixel_shader() const override;
    config_node const& parameters() const override;
    config_node const& sampler_states() const override;

private: // data members
    std::string m_name;
    std::string m_pixel_hlsl;
    config_node m_parameters;
    config_node m_sampler_states;
};

//class point_light_shader sealed : public light_shader
//{
//public:
//    point_light_shader();
//
//public: // interface required by light_shader
//    std::string const& name() const override;
//    std::string const& pixel_shader() const override;
//    config_node const& parameters() const override;
//    config_node const& sampler_states() const override;
//
//private: // data members
//    std::string m_pixel_hlsl;
//};

class dummy_light_shader sealed : public light_shader
{
public:
    dummy_light_shader();

public: // interface required by light_shader
    std::string const& name() const override;
    std::string const& pixel_shader() const override;
    config_node const& parameters() const override;
    config_node const& sampler_states() const override;

private: // data members
    std::string m_name;
    std::string m_pixel_hlsl;
    config_node m_empty_config;
};

//class frustum_light_shader sealed : public light_shader
//{
//public:
//    frustum_light_shader();
//
//public: // interface required by light_shader
//    std::string const& name() const override;
//    std::string const& pixel_shader() const override;
//    config_node const& parameters() const override;
//    config_node const& sampler_states() const override;
//
//private: // data members
//    std::string m_pixel_hlsl;
//};
//
//class shadow_mapped_frustum_light_shader sealed : public light_shader
//{
//public:
//    shadow_mapped_frustum_light_shader();
//
//public: // interface required by light_shader
//    std::string const& name() const override;
//    std::string const& pixel_shader() const override;
//    config_node const& parameters() const override;
//    config_node const& sampler_states() const override;
//
//private: // data members
//    std::string m_pixel_hlsl;
//};

std::unique_ptr<basic_light_shader> load_light_shader(pt::path const& path);

}}} // namespace pt::d3d9::shaders

#endif
