
#ifndef COCOEXTENSIONS_COLLIDERRECTDRAWER_h
#define COCOEXTENSIONS_COLLIDERRECTDRAWER_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/cocostudio.h"

/**
 *  PrimitiveLayer is inherited from CCLayer, using for rendering all primitive
 */
class  ColliderRectDrawer : public cocos2d::Layer
{
public:
    static ColliderRectDrawer *create(cocostudio::Armature *armature);
public:
    ColliderRectDrawer();
    /**
     *	@brief renders Primitive
     */
    //virtual void draw();
private:
    CC_SYNTHESIZE(cocostudio::Armature*, m_pArmature, Armature);
};


#endif
