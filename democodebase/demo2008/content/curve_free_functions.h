#ifndef CONTENT_CURVE_FREE_FUNCTIONS_H
#define CONTENT_CURVE_FREE_FUNCTIONS_H

#include "pt/config_node_fwd.h"
#include "pt/std/memory.h"

namespace pt {
    class parameter_node;
    class parameter_collection;
}

namespace pt { namespace math {
    template<typename ResultType>
    class spline_base;
}}

namespace content {

using namespace pt;

std::shared_ptr<parameter_node> create_spline(config_node const& config);
std::shared_ptr<parameter_node> create_optional_reference_curve(
    config_node const& config,
    parameter_collection& existing_curves);

typedef math::spline_base<float> splinef;

} // namespace content

#endif
