#include "curve_loader.h"
#include "pt/std/memory.h"
#include "pt/parameter_node.h"
#include "pt/path.h"

#include "pt/directory_iterator.h"
#include "pt/boost/filesystem/convenience.h"

using namespace pt;

namespace {
    /// Recursively loads material descriptions under \a path into \a materials.
    void load_curves(boost::filesystem::path const& path, curves_collection& materials)
    {
        // directory_iterator default construction yields past-the-end
        pt::directory_iterator end_iter;

        log(format("load_materials path: \"%1%\" ") % path.string());

        for (pt::directory_iterator itr(path); itr != end_iter; ++itr)
        {
            log(format("->: \"%1%\" ") % itr->string());
                    
            //const std::string *str = (const std::string *)&(*itr);
            //if (strncmp(str->c_str(), "data/materials", strlen("data/materials")) != 0)
            //{
            //	log("break;");
            //	break;
            //}

            if (filesystem::is_directory(*itr))
            {
                load_materials(*itr, materials);
                continue;
            }
            
            if (itr->extension() == ".xml")
            {
                materials.add(pt::path(itr->string()));
            }
        }
    }
} // anonymous namespace


namespace content {

curve_loader::curve_loader()
{
}

std::shared_ptr<parameter_node> load_curves(pt::path const& path)
{
    std::shared_ptr<parameter_node> result();

    return result;
}

} // namespace content
