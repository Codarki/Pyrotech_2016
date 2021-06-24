#pragma once
#ifndef PT_PARAMETER_COLLECTION_H
#define PT_PARAMETER_COLLECTION_H

#include "parameter_collection_fwd.h"
#include "pt/std/memory.h"
#include "pt/std/list.h"
#include "pt/std/string.h"

namespace pt {

class parameter_node;
class string_wrapper;

/// Collection of parameter nodes.
class parameter_collection
{
private: // type definitions
    typedef std::list<std::shared_ptr<parameter_node> > container;
public:
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    /// Throws if not found.
    const_iterator child(string_wrapper const& name) const;
    /// Returns end() if not found.
    const_iterator find_child(string_wrapper const& name) const;

    void add(std::shared_ptr<parameter_node> node);
    void remove(std::shared_ptr<parameter_node> node);

    size_t size() const;
    bool empty() const;

private: // helper functions
    iterator find(std::shared_ptr<parameter_node> node);

private: // data members
    std::string m_name;
    container m_children;
};

std::shared_ptr<parameter_node> find_child(std::string const& name,
    std::string const& value, parameter_collection const& parameters);

/// Returns nothing if not found.
std::shared_ptr<parameter_node> optional_child_node(std::string const& name,
    parameter_collection const& parameters);

} // namespace pt

#endif
