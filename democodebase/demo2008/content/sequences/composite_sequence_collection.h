#ifndef CONTENT_SEQUENCES_COMPOSITE_SEQUENCE_COLLECTION_H
#define CONTENT_SEQUENCES_COMPOSITE_SEQUENCE_COLLECTION_H

#include "pt/std/memory.h"
#include "pt/std/vector.h"

namespace content { namespace sequences {

class composite_sequence;

class composite_sequence_collection
{
public:
    std::vector<std::shared_ptr<composite_sequence> > const& get() const
    {
        return sequences_;
    }

    std::vector<std::shared_ptr<composite_sequence> >& get()
    {
        return sequences_;
    }

    void set(std::vector<std::shared_ptr<composite_sequence> > const& x)
    {
        sequences_ = x;
    }

private:
    std::vector<std::shared_ptr<composite_sequence> > sequences_;
};

}} // namespace content::sequences

#endif
