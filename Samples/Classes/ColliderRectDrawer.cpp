
#include "ColliderRectDrawer.h"

ColliderRectDrawer *ColliderRectDrawer::create(cocostudio::Armature *armature)
{
    ColliderRectDrawer *drawer = new ColliderRectDrawer();
    if (drawer && drawer->init())
    {
        drawer->setArmature(armature);

        drawer->autorelease();
        return drawer;
    }
    CC_SAFE_DELETE(drawer);
    return NULL;
}

ColliderRectDrawer::ColliderRectDrawer()
    : m_pArmature(NULL)
{

}

//void ColliderRectDrawer::draw()
//{
//    if (m_pArmature)
//    {
//        m_pArmature->drawContour();
//    }
//}
