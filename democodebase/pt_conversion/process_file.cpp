#include "process_file.h"
#include "direct_copy.h"
#include "p3d/write.h"
#include "pt/config_node.h"
#include "pt/data_input_stream.h"
#include "pt/file_input_stream.h"
#include "pt/log.h"
#include "pt/verbose.h"
#include "pt/format.h"
#include "file_3ds/reader.h"
#include "file_3ds/data.h"

namespace pt { namespace conversion {

void process_file(path const& src, path const& dst, path const& dst_root)
{
    log(format("Converting file %1% into %2%") % src.string() % dst.string());
    logging::scoped_indent indent;

    if (src.extension() == ".3ds" || src.extension() == ".3DS")
    {
        //pt::enable_verbose(false);

        config_node config;
        path asset_path = src;
        asset_path.replace_extension(".pt_asset.xml");
        if (pt::exists(asset_path))
            config = config_node(asset_path);

        data_input_stream stream
            = data_input_stream_from(file_input_stream(src));

		boost::shared_ptr<file_3ds::data> data = file_3ds::read(stream);

        p3d::write(dst, dst_root/"objects", dst_root/"images", data, config);
        pt::enable_verbose(true);
    }
    else
    {
        // direct copy
        direct_copy(src, dst);
    }
}

}} // namespace pt::conversion
