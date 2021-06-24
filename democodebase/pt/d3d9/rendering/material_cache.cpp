#include "material_cache.h"
#include "surface_cache.h"
#include "source/material_collection.h"
#include "pt/std/cassert.h"

namespace pt { namespace d3d9 { namespace rendering {

material_cache::material_cache(parameter_collection& shared_parameters)
: m_shared_parameters(&shared_parameters)
{
}

material_cache::~material_cache()
{
}

std::string material_cache::parameters_as_readable_text() const
{
    return m_surfaces.parameters_as_readable_text();
}

parameter_collection& material_cache::shared_parameters()
{
    return *m_shared_parameters;
}

void material_cache::load_materials(pt::path const& path)
{
    assert(!m_materials && "material insertion not implemented yet");
    m_materials = source::load_materials(path);
}

std::shared_ptr<surface> material_cache::find_or_create(
    graphics_device& device, parameter_collection const& parameters)
{
    assert(m_materials);
    std::shared_ptr<source::material> mat
        = m_materials->populate(parameters, *m_shared_parameters, device);

    return m_surfaces.find_or_create(mat);
}

std::shared_ptr<surface> material_cache::find_or_create(graphics_device& device,
    std::string const& name, parameter_collection const& parameters)
{
    assert(m_materials);
    std::shared_ptr<source::material> mat
        = m_materials->populate(name, parameters, *m_shared_parameters,
        device);

    return m_surfaces.find_or_create(mat);
}

std::shared_ptr<light_properties> material_cache::find_or_create_light(
    graphics_device& device,
    parameter_collection const& parameters)
{
    assert(m_materials);
    std::shared_ptr<source::light_material> mat
        = m_materials->populate_light(parameters, *m_shared_parameters, device);

    return m_light_properties.find_or_create(mat);
}

}}} // namespace pt::d3d9::rendering
