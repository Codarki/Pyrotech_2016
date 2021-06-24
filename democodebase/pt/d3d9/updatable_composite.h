#ifndef PT_D3D9_RENDERING_UPDATABLE_COMPOSITE_H
#define PT_D3D9_RENDERING_UPDATABLE_COMPOSITE_H

//#include "updatable.h"
//#include "pt/std/vector.h"

//namespace pt { namespace d3d9 { namespace rendering {
//
///// Composite of updatables.
//class updatable_composite : public updatable
//{
//public:
//    void push_back(shared_ptr<updatable> updatable)
//    {
//        m_updatables.push_back();
//    }
//
//public: // interface required by updatable
//    void update(float time)
//    {
//        for (std::vector<shared_ptr<updatable> >::iterator i
//            = m_updatables.begin(); i != m_updatables.end();++i)
//        {
//            (*i)->update(time);
//        }
//    }
//
//private: // data members
//    std::vector<shared_ptr<updatable> > m_updatables;
//};

}}} // namespace pt::d3d9::rendering

#endif
