#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "cocos-ext.h"
#include "ReadJSHelper.h"
#include "ColliderRectDrawer.h"
#include "cocostudio/TriggerBase.h"
#include "trigger/EventDef.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
	sendEvent(TRIGGEREVENT_ENTERSCENE);
}

void HelloWorld::onExit()
{
	CCLayer::onExit();
	sendEvent(TRIGGEREVENT_LEAVESCENE);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	if (ReadJSHelper::Instance()->GetRunType() == e_FlushSceneRun)
	{
		CCNode *pNode = NULL;
		cocostudio::SceneReader::getInstance()->setTarget(CC_CALLBACK_2(HelloWorld::comCallBack, this));
		pNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile(ReadJSHelper::Instance()->GetJsonPath().c_str());
		if (pNode != NULL)
		{
			this->addChild(pNode);
		}
	}
	else if (ReadJSHelper::Instance()->GetRunType() == e_FlushUIRun)
	{
        cocos2d::CSLoader* cache = cocos2d::CSLoader::getInstance();
        cache->setRecordJsonPath(false);
        Node* node = cache->createNode(ReadJSHelper::Instance()->GetJsonPath().c_str());

		cocostudio::timeline::ActionTimeline* action = cocostudio::timeline::ActionTimelineCache::getInstance()->createAction(ReadJSHelper::Instance()->GetJsonPath().c_str());
		if(action)
        {
            node->runAction(action);
            action->gotoFrameAndPlay(0);
        }

        this->addChild(node);
	}
	else if (ReadJSHelper::Instance()->GetRunType() == e_FlushCSBRun)
	{
        cocos2d::CSLoader* cache = cocos2d::CSLoader::getInstance();
		cache->setRecordProtocolBuffersPath(false);
		Node* node = cache->createNodeFromProtocolBuffers(ReadJSHelper::Instance()->GetJsonPath().c_str());        
		
		cocostudio::timeline::ActionTimeline* action = cocostudio::timeline::ActionTimelineCache::getInstance()->createActionFromProtocolBuffers(ReadJSHelper::Instance()->GetJsonPath().c_str());
		if(action)
        {
            node->runAction(action);
            action->gotoFrameAndPlay(0);
        }		

        this->addChild(node);
	}
	else if (ReadJSHelper::Instance()->GetRunType() == e_FlushCSDRun)
	{
		cocos2d::Node* node = cocos2d::CSLoader::createNode(ReadJSHelper::Instance()->GetCSDResPath());
		cocostudio::timeline::ActionTimeline* action = cocostudio::timeline::ActionTimelineCache::createAction(ReadJSHelper::Instance()->GetCSDResPath());
        
        if (node && action)
        {
            node->runAction(action);
            action->gotoFrameAndPlay(0);
        }
        this->addChild(node);
        
        if(ReadJSHelper::Instance()->GetCSDType() == "Node")
        {
            setPosition(_contentSize.width/2, _contentSize.height/2);
        }
		
        
        /*
        cocos2d::CSLoader* cache = cocos2d::CSLoader::getInstance();
        cache->setRecordXMLPath(false);
        
        Node* node = cache->createNodeFromXML(ReadJSHelper::Instance()->GetCSDResPath());
        node->setAnchorPoint(Vec2(0.0f, 0.0f));
        cocostudio::timeline::ActionTimeline* action = cocostudio::timeline::ActionTimelineCache::getInstance()->createActionFromXML(ReadJSHelper::Instance()->GetCSDResPath());
        if(action)
        {
            node->runAction(action);
            action->gotoFrameAndPlay(0);
        }
        this->addChild(node);

        if(ReadJSHelper::Instance()->GetCSDType() == "Node")
        {
            setPosition(_contentSize.width/2, _contentSize.height/2);
        }

//         */
        
        /*
		ProtocolBuffersSerialize* pbs = ProtocolBuffersSerialize::getInstance();
		std::string csbPath = ReadJSHelper::Instance()->GetJsonPath().c_str();
		
        std::string result = pbs->serializeProtocolBuffersWithXMLFile(ReadJSHelper::Instance()->GetCSDResPath(), ReadJSHelper::Instance()->GetJsonPath().c_str());
//		pbs->set_protocolbuffersDir(ReadJSHelper::Instance()->GetResources() + "/");
//		std::string result = pbs->serializeProtocolBuffersWithXMLFile(ReadJSHelper::Instance()->GetCSDResPath(), ReadJSHelper::Instance()->GetJsonPath().c_str(), true);
        cocos2d::CSLoader* cache = cocos2d::CSLoader::getInstance();
	    cache->setRecordProtocolBuffersPath(false);
		if (result == "")
		{
			Node* node = cache->createNode(ReadJSHelper::Instance()->GetCSDResPath());
		    cocostudio::timeline::ActionTimeline* action = cocostudio::timeline::ActionTimelineCache::getInstance()->createAction(ReadJSHelper::Instance()->GetCSDResPath());
		    if(action)
            {
                node->runAction(action);
                action->gotoFrameAndPlay(0);
			}
            this->addChild(node);

         */
	}
	
	
	sendEvent(TRIGGEREVENT_INITSCENE);
	this->schedule(schedule_selector(HelloWorld::gameLogic));
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);
    return true;
}

void HelloWorld::comCallBack(cocos2d::Ref *tar, void *dict)
{
	if (dict == NULL || tar == NULL)
	{
		return;
	}

	ComRender *pRender = dynamic_cast<ComRender*>(tar);
	if (pRender == NULL)
	{
		return;
	}
	Armature *pAr = dynamic_cast<Armature*>(pRender->getNode());
	if (pAr == NULL)
	{
		return;
	}
	rapidjson::Value *v = (rapidjson::Value *)dict;
    bool isShowColliderRect = DICTOOL->getBooleanValue_json(*v, "isShowColliderRect");
	if (isShowColliderRect == false)
	{
		return;
	}
	
	ColliderRectDrawer *drawer = ColliderRectDrawer::create(pAr);
	if (pAr->getParent())
	{
		pAr->getParent()->addChild(drawer);
	}
	drawer->setVisible(true);
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    sendEvent(TRIGGEREVENT_TOUCHBEGAN);
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
    sendEvent(TRIGGEREVENT_TOUCHMOVED);
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
    sendEvent(TRIGGEREVENT_TOUCHENDED);
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *unused_event)
{
    sendEvent(TRIGGEREVENT_TOUCHCANCELLED);
}

void HelloWorld::gameLogic(float dt)
{
    sendEvent(TRIGGEREVENT_UPDATESCENE);
}
