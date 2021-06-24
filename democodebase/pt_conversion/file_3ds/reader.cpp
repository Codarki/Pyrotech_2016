#include "reader.h"
#include "data.h"
#include "common.h"
#include "pt/bounded_input_stream.h"

namespace pt { namespace conversion { namespace file_3ds {

boost::shared_ptr<data> read(bounded_input_stream& s)
{
	boost::shared_ptr<data> d(new data());

    while (s.position() < s.size())
    {
        chunk_info info;
        read_chunk_info(info, s);

        if (info.is(CHUNK_MAIN))
        {
            d->read(info, s);
        }

        skip_chunk(info, s);
    }

    return d;
}

}}} // namespace pt::conversion::3ds
