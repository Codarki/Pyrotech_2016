#ifndef PT_D3D9_RENDERING_UPDATABLE_H
#define PT_D3D9_RENDERING_UPDATABLE_H

namespace pt { namespace d3d9 { namespace rendering {

/// Abstract base class for 3d engine updatables.
class updatable
{
public:
    virtual ~updatable() {}

public: // interface which subclasses must implement
    virtual void update(float time) = 0;
};

}}} // namespace pt::d3d9::rendering

#endif
