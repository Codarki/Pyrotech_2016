#include "readers.h"
#include "chunk_reader.h"
#include "mesh.h"
#include "scene.h"
#include "pt/data_input_stream.h"
#include "pt/file_input_stream.h"
#include "pt/config_node.h"
#include "pt/path.h"
#include "pt/log.h"
#include "pt/format.h"

namespace p3d_format {


boost::shared_ptr<scene> read_scene(pt::path const& path, progress_report* progress,
    bool only_serialize)
{
    // Open and read copy of .p3d_scene file.

    pt::config_node scene_config;
    pt::path asset_path = path;
    asset_path.replace_extension(".pt_asset.xml");
    pt::log(pt::format("Checking for %1%") % asset_path.string());
    if (pt::immutable_file_exists(asset_path))
    {
        pt::log(pt::format("Found %1%") % asset_path.string());
        pt::config_node config = pt::config_node(asset_path);
        // Hack around the root element. Actual XML root <asset> is the first child.
        auto iter = config.begin();
        if (iter != config.end())
        {
            pt::log(pt::format("%1% has some content") % asset_path.string());

            config_node child_config = *iter;
            config = child_config;
        }

        // Get the <asset><scene> XML element. Throws if not found.
        // config = *config.child("scene") throws std::bad_alloc, fix it, should work.
        scene_config = *config.child("scene");
        pt::log(pt::format("%1% found scene element.") % asset_path.string());
    }

    std::shared_ptr<data_input_stream> stream = shared_data_input_stream_from(
        file_input_stream(path));

    chunk_reader chunk(stream);

    return make_boost_shared_ptr(new scene(chunk, scene_config, progress, only_serialize));
}

boost::shared_ptr<mesh> read_mesh(pt::path const& path, progress_report* progress)
{
    // Open and read copy of .p3d_model file.

    std::shared_ptr<data_input_stream> stream = shared_data_input_stream_from(
        file_input_stream(path));

    chunk_reader chunk(stream);
    return make_boost_shared_ptr(new mesh(chunk, path.string(), progress));
}

} // namespace p3d_format
