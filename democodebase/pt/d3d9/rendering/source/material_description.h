#ifndef PT_D3D9_RENDERING_SOURCE_MATERIAL_DESCRIPTION_H
#define PT_D3D9_RENDERING_SOURCE_MATERIAL_DESCRIPTION_H

#include "pt/config_node.h"
#include "pt/path.h"
#include "pt/std/memory.h"
#include "pt/std/string.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering { namespace source {

class shader_description
{
public:
    shader_description(pt::path const& shader_path,
        config_node const& parameters, pt::path const& pre_shader_path);

    pt::path const& shader_path() const;
    pt::path const& pre_shader_path() const;
    config_node const& parameters() const;

private: // data members
    pt::path m_shader_path;
    pt::path m_pre_shader_path;
    config_node m_parameters;
};

/// Description of a material for rendering engine.
/// Immutable.
class material_description
{
public:
    material_description(std::string const& name,
        std::string const& p3d_name,
        std::vector<shader_description> shader_description);

    std::string const& name() const;
    std::string const& p3d_name() const;

    std::vector<shader_description> const& shader_descriptions() const;

private: // data members
    std::string m_name;
    std::string m_p3d_name;
    std::vector<shader_description> m_shader_description;
};

/// Builds material descriptions.
/// Decouples building of material description.
class material_description_builder
{
public:
    std::unique_ptr<material_description> build_unique(
        config_node const& material) const;

    shader_description build_shader_description(
        config_node const& config, std::string material_name) const;
};

std::unique_ptr<material_description> build_unique_light_material_description(
    config_node const& material);

}}}} // namespace pt::d3d9::rendering::source

#endif
