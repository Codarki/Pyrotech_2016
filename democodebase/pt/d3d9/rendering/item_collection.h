#pragma once
#ifndef PT_D3D9_RENDERING_ITEM_COLLECTION_H
#define PT_D3D9_RENDERING_ITEM_COLLECTION_H

#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace pt { namespace d3d9 { namespace rendering {

class item;

class item_collection
{
private: // type definitions
	typedef std::vector<std::shared_ptr<item> > container;
public:
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

	void push_back(std::shared_ptr<item> child);
    void erase(std::shared_ptr<item> const& child);

    iterator find(std::shared_ptr<item> const& child);

private: // data members
    container m_children;
};

}}} // namespace pt::d3d9::rendering

#endif
