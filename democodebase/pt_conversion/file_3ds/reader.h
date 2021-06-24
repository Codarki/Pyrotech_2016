#ifndef PT_CONVERSION_FILE_3DS_READER_H
#define PT_CONVERSION_FILE_3DS_READER_H

#include "pt/boost/shared_ptr.h"

namespace pt {
    class bounded_input_stream;
} // namespace pt

namespace pt { namespace conversion { namespace file_3ds {

class data;

/// Reads a .3ds file from \a stream.
boost::shared_ptr<data> read(bounded_input_stream& stream);

}}} // namespace pt::conversion::file_3ds

#endif
