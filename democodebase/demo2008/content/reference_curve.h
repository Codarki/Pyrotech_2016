#ifndef CONTENT_REFERENCE_STEP_CURVE_H
#define CONTENT_REFERENCE_STEP_CURVE_H

namespace content {

using namespace pt;

class reference_step_curve : public animatable_value_source<std::string>
{
public:
    typedef std::shared_ptr<animatable_value_source<std::string> > value_type;

private: // data members
};

} // namespace content

#endif
