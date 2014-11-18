#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	 
	/**
     *  @js NA
     *  @lua NA
     */
	virtual void onEnter();

    /**
     *  @js NA
     *  @lua NA
     */
	virtual void onExit();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();

    // CallBack
	void comCallBack(cocos2d::Ref *tar, void *dict);

	// default implements are used to call script callback if exist
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	// gameLogic
	void gameLogic(float dt);

	// implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
