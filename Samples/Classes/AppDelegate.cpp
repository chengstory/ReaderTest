#include "AppDelegate.h"

#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "ReadJSHelper.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {
	
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview)
	{
        if (ReadJSHelper::Instance()->GetRunType() == FlushType::e_FlushCSBRun)
        {
            glview = GLViewImpl::create("Simulator_csb");
        }
        else
        {
			glview = GLViewImpl::create("Simulator");
        }
		
		glview->setFrameSize(ReadJSHelper::Instance()->GetWinSize().width, ReadJSHelper::Instance()->GetWinSize().height);
		director->setOpenGLView(glview);
	}
	// turn on display FPS
	director->setDisplayStats(true);
  
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::scene();

    // run
    director->runWithScene(scene);

	cocos2d::log("iShow!");
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
