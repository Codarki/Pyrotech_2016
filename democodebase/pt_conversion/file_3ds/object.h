#ifndef PT_CONVERSION_FILE_3DS_OBJECT_H
#define PT_CONVERSION_FILE_3DS_OBJECT_H

#include "pt/std/string.h"
#include "pt/boost/shared_ptr.h"

namespace pt {
class bounded_input_stream;
    
namespace conversion { namespace file_3ds {

struct chunk_info;

using namespace pt;

/// Abstract base class for all 3ds objects (mesh, light or camera).
class object
{
public:
    object(std::string const& name);
    std::string const& name() const;

    virtual bool is_mesh() const = 0;
    virtual bool is_light() const = 0;
    virtual bool is_camera() const = 0;

private: // data members
    std::string m_name;
};

boost::shared_ptr<object> read_object(chunk_info const& parent_info,
    bounded_input_stream& s);

}}} // namespace pt::conversion::file_3ds

#endif
