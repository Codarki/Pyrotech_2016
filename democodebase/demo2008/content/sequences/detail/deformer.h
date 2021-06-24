#ifndef CONTENT_SEQUENCES_DETAIL_DEFORMER_H
#define CONTENT_SEQUENCES_DETAIL_DEFORMER_H

#include "pt/matrix.h"
#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 {
    class graphics_device;
    class vertex_streams;

    namespace rendering {
        class item;
    }
}}

namespace content { namespace sequences { namespace detail {

using namespace pt;

class deformer
{
public:
    deformer();

    void set_parent_to_rest(matrix4x4f const& m);
    matrix4x4f const& parent_to_rest() const;

    float length() const;

	void set_item(std::shared_ptr<d3d9::rendering::item> item);
    std::shared_ptr<d3d9::rendering::item const> item() const;

private: // data members
    float m_length;
    matrix4x4f m_local_transformation;

    std::shared_ptr<d3d9::rendering::item> m_item;
    matrix4x4f m_parent_to_rest;
};

class deformer_collection
{
private: // type definitions
    typedef std::vector<deformer> container;
public:
    typedef container::const_iterator const_iterator;

public:
    deformer_collection();

    void push_back(deformer const& deformer);

    size_t size() const;
    deformer const& at(size_t index) const;

    const_iterator begin() const;
    const_iterator end() const;

private: // data members
    std::vector<deformer> m_deformers;
};

}}} // namespace content::sequences::detail

#endif
