#include "deformer.h"

namespace content { namespace sequences { namespace detail {

deformer::deformer()
:   m_length(2)
{
}

void deformer::set_parent_to_rest(matrix4x4f const& m)
{
    m_parent_to_rest = m;
}

matrix4x4f const& deformer::parent_to_rest() const
{
    return m_parent_to_rest;
}

float deformer::length() const
{
    return m_length;
}

void deformer::set_item(std::shared_ptr<d3d9::rendering::item> item)
{
    m_item = item;
}

std::shared_ptr<d3d9::rendering::item const> deformer::item() const
{
    return m_item;
}

// deformer_collection

deformer_collection::deformer_collection()
{
}

void deformer_collection::push_back(deformer const& deformer)
{
    m_deformers.push_back(deformer);
}

size_t deformer_collection::size() const
{
    return m_deformers.size();
}

deformer const& deformer_collection::at(size_t index) const
{
    assert(index < size());
    return m_deformers[index];
}

deformer_collection::const_iterator deformer_collection::begin() const
{
    return m_deformers.begin();
}

deformer_collection::const_iterator deformer_collection::end() const
{
    return m_deformers.end();
}

}}} // namespace content::sequences::detail
