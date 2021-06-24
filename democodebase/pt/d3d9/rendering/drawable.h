#ifndef PT_D3D9_RENDERING_DRAWABLE_H
#define PT_D3D9_RENDERING_DRAWABLE_H

namespace pt { namespace d3d9 { namespace rendering {

class surface;

/// Abstract base class for drawables.
class drawable
{
public:
    virtual ~drawable() {};

public: // interface which subclasses must implement
    virtual void draw() const = 0;
    //virtual rendering::surface const& surface() const = 0;
    //virtual rendering::surface& surface() = 0;
};

}}} // namespace pt::d3d9::rendering

#endif
