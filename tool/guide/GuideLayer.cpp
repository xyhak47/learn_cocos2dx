#include "GuideLayer.h"
#include "components/guide/GuideEvent.h"
#include "components/guide/GuideNode.h"
#include "UI/UIResourceFile.h"
#include "2d/GCSprite.h"
#include "components/guide/GuideHelper.h"
#include "Common/CocoUIUtils.h"

USING_NS_CC;
USING_NS_GC;

GuideLayer::GuideLayer()
:defaultColor(0, 0, 0, 255/2)
,m_exitHand(false)
{
    setContentSize(VisibleRect::getVisibleRect().size);
    addListener();
}

GuideLayer::~GuideLayer()
{
    removeListener();
}

GuideLayer* GuideLayer::create(const Rect& rect)
{
	GuideLayer* ret = new GuideLayer();
	if(ret && ret->init(rect))
    {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

GuideLayer* GuideLayer::create(const Rect& rect,const Color4B& color)
{
	GuideLayer* ret = new GuideLayer();
	if(ret && ret->init(rect,color))
    {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool GuideLayer::init(const Rect& rect)
{
	if(!Node::init())
    {
        return false;
    }
    
    m_rect = rect;
//    addMask(rect);
//    addCliping();

	return true;
}


bool GuideLayer::init(const Rect& rect,const Color4B& color)
{
	Color4B temp = defaultColor;
	defaultColor = color;
	if(init(rect))
    {
		defaultColor = temp;
		return true;
	}
    else
    {
		return false;
	}
}

bool GuideLayer::onTouchBegan(Touch* touch,Event* event)
{
    Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
    if(m_rect.containsPoint(touchPoint))
    {
        m_listener->setSwallowTouches(false);
    }
    else
    {
        m_listener->setSwallowTouches(true);
    }
    return true;
}

void GuideLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event)
{
//    Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
//    if(m_rect.containsPoint(touchPoint))
//    {
//        m_listener->setSwallowTouches(false);
//    }
//    else
//    {
//        m_listener->setSwallowTouches(true);
//    }
}

void GuideLayer::onTouchEnded(Touch* touch,Event* event)
{
   	Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
    if(m_rect.containsPoint(touchPoint))
    {
        DISPATCH_EVENT(GUIDE_EVENT_TOUCH_IN_RECT);
    }
}

void GuideLayer::wait()
{
    m_exitHand = false;
    removeAllChildren();
    m_rect = Rect(-100,-100,0,0);
    addMask(false);
}

void GuideLayer::reset()
{
    m_exitHand = false;
    removeAllChildren();
    addMask();
}

void GuideLayer::resetCliping(const RectInfo& rectInfo)
{
    m_exitHand = false;
    removeAllChildren();

    addCliping(rectInfo);
}

void GuideLayer::addMask(bool show)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    LayerColor* color = LayerColor::create(defaultColor);
    color->setContentSize(visibleSize);
    addChild(color);
    color->setVisible(show);
}

void GuideLayer::addCliping(const RectInfo& rectInfo)
{
    ClippingNode* clipper = ClippingNode::create();
    clipper->setContentSize(VisibleRect::getVisibleRect().size);
    addChild(clipper);
    clipper->setAnchorPoint(Point(0,0));

    LayerColor* layerBg = LayerColor::create(defaultColor);
    clipper->addChild(layerBg);
    layerBg->setContentSize(VisibleRect::getVisibleRect().size);
    layerBg->setAnchorPoint(Point(0.5,0.5));
    layerBg->setScale(4.0f);
    
    Node* stencil = GuideHelper::getInstance()->getNodeFromMap(rectInfo.getName());
    
    // special treatment here, ugly hard code, by xyh
    if (rectInfo.getName() == GUIDE_CLIPING_BATTLE_TIP_GET_REWARD ||
        rectInfo.getName() == GUIDE_CLIPING_BATTLE_TIP_CHALLENGE
        )
    {
        m_rect.size = stencil->getContentSize();
        Point world_pt = stencil->getParent()->convertToWorldSpace(stencil->getPosition());
        m_rect.origin = world_pt - stencil->getContentSize()/2 + Point(0,40);
        clipper->setPosition(world_pt - stencil->getContentSize()/2 -stencil->getContentSize()*0.05);
        clipper->setScale(1.10f);
    }
    else if (rectInfo.getName() == "GUIDE_CLIPING_BATTLE_TIP0" ||
             rectInfo.getName() == "GUIDE_CLIPING_BATTLE_TIP1" ||
             rectInfo.getName() == "GUIDE_CLIPING_BATTLE_TIP2"
             ) 
    {
        m_rect.size = stencil->getContentSize() * 0.8;
        m_rect.origin = CocoUIUtils::getNodePositionInParentLocatedByAnchor(stencil, Point()) + Point(0,30);
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_BTN_HEROLIST_HEAD_MOVE_FIRST
            || rectInfo.getName() == GUIDE_CLIPING_BTN_HEROLIST_HEAD_MOVE_SEC
             )
    {
        m_rect.size = stencil->getContentSize();
        Point world_pt = stencil->getParent()->convertToWorldSpace(stencil->getPosition());
        m_rect.origin = world_pt - stencil->getContentSize()/2;
        clipper->setVisible(false);
    }
    else if (   rectInfo.getName() == GUIDE_CLIPING_BTN_HEROLIST
             || rectInfo.getName() == GUIDE_CLIPING_BTN_SUMMON
             || rectInfo.getName() == GUIDE_CLIPING_BTN_FRIEND
             )
    {
        m_rect.size = stencil->getContentSize();
        Point world_pt = stencil->getParent()->convertToWorldSpace(stencil->getPosition());
        m_rect.origin = world_pt - stencil->getContentSize()/2 + Point(0,10);
        clipper->setVisible(false);
        addMaskAround();
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_SUMMON_MASK)
    {
        m_rect.size = Size(190, VisibleRect::getVisibleRect().size.height - 60);
        m_rect.origin = Point(VisibleRect::getVisibleRect().size.width/2 - 95, 0);
        clipper->setVisible(false);
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_SUMMON_BTN_CHANGE_RIGHT)
    {
        m_rect.size = stencil->getContentSize();
        m_rect.origin  = Point(VisibleRect::getVisibleRect().size.width, 0) + Point(-537,69) - stencil->getContentSize()/2;
    }
    else if (   rectInfo.getName() == GUIDE_CLIPING_HEROLIST_HERO_HEAD
             || rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_UPGRADE_RECT
             || rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_GO_BATTLE
             || rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_STAR_UP_RECT
             || rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_EQUIP_UP_RECT
             || rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_RUNE
             || rectInfo.getName() == GUIDE_CLIPING_BATTLE_BTN_INVADE_HEAD
             || rectInfo.getName() == GUIDE_CLIPING_BATTLE_BTN_INVADE_BEGINE
             || rectInfo.getName() == GUIDE_CLIPING_HALL_DISPATCHE_TEAM_FIRSRT
             || rectInfo.getName() == GUIDE_CLIPING_HALL_DISPATCHE_SURE
             || rectInfo.getName() == GUIDE_CLIPING_HALL_DISPATCHE_VIEW_BTN
             )
    {
        m_rect.size = stencil->getContentSize();
        Point world_pt = stencil->getParent()->convertToWorldSpace(stencil->getPosition());
        m_rect.origin = world_pt - stencil->getContentSize()/2;
        clipper->setVisible(false);
        addMaskAround();
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_HALL_DISPATCHE_BTN)
    {
        m_rect.size = stencil->getContentSize();
        // ipad is Point(0.5,0.5) iphone is Point(0.5,0)
        m_rect.origin = CocoUIUtils::getNodePositionInParentLocatedByAnchor(stencil, Point(0.5,0));
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_UPGRADE_ROUND)
    {
        m_rect.size = stencil->getContentSize();
        m_rect.origin = Point(VisibleRect::getVisibleRect().size/2) + Point(400,220);
        clipper->setVisible(false);
        addMaskAround();
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_STAR_UP)
    {
        m_rect.size = stencil->getContentSize();
        m_rect.origin = Point(VisibleRect::getVisibleRect().size/2) + Point(390,190);
        clipper->setVisible(false);
        addMaskAround();
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_HEROLIST_BTN_EQUIP_UP)
    {
        m_rect.size = stencil->getContentSize();
        m_rect.origin = Point(VisibleRect::getVisibleRect().size/2) + Point(365,140);
        clipper->setVisible(false);
        addMaskAround();
    }
    else if (rectInfo.getName() == GUIDE_CLIPING_BATTLE_BTN_INVADE_SEARCH)
    {
        m_rect.size = Size(100,100);
        m_rect.origin = Point(VisibleRect::getVisibleRect().size.width-250,100);
    }
    else
    {
        m_rect.size = stencil->getContentSize();
        m_rect.origin = CocoUIUtils::getNodePositionInParentLocatedByAnchor(stencil, Point());
    }
    
    // for test
//    LayerColor* lc = LayerColor::create(Color4B(255,0,0,255));
//    lc->setContentSize(m_rect.size);
//    lc->setPosition(m_rect.origin);
//    lc->setAnchorPoint(Point());
//    clipper->addChild(lc, 10000);


    clipper->setStencil(stencil);
    clipper->setInverted(true);
    clipper->setAlphaThreshold(0.5f);
}

void GuideLayer::addListener()
{
    // add touch listener
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->onTouchBegan = CC_CALLBACK_2(GuideLayer::onTouchBegan,this);
    m_listener->onTouchMoved = CC_CALLBACK_2(GuideLayer::onTouchMoved,this);
    m_listener->onTouchEnded = CC_CALLBACK_2(GuideLayer::onTouchEnded,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener,this);
    
    // add custom child event
    auto func = [this](EventCustom* event)
    {
        if (m_exitHand == false)
        {
            DISPATCH_EVENT(GUIDE_EVENT_CONTENUE);
        }
    };
    
    ADD_LISTENER(GUIDE_EVENT_TALK_TOUCH, func);
}

void GuideLayer::removeListener()
{
    // remove touch listener
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_listener);
    
    // remove custom child event
    REMOVE_LISTENER(GUIDE_EVENT_TALK_TOUCH);
}

void GuideLayer::showHandClick(const HandInfo& info)
{
    m_exitHand = true;
    HandInfo temp = info;
    temp.setHandPos(m_rect.origin);
    temp.setVisiableRectSize(m_rect.size);
    GuideNode::showHand(this, temp);
}

void GuideLayer::showHandMove(const HandInfo& info)
{
    m_exitHand = true;
    HandInfo temp = info;
    temp.setHandPos(m_rect.origin);
    temp.setVisiableRectSize(m_rect.size);
    GuideNode::showHandMove(this, temp);
}

void GuideLayer::showTalk(const TalkInfo& info)
{
    GuideNode::showTalk(this, info);
}

void GuideLayer::showDialog(const DialogInfo& info)
{
    GuideNode::showDialog(this, info);
}

void GuideLayer::addMaskAround()
{
    Rect rect = m_rect;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    area1 = LayerColor::create(
                               defaultColor,
                               visibleSize.width,
                               visibleSize.height - rect.getMaxY()
                               );
    area1->setAnchorPoint(Point(0,0));
    area1->setPosition(Point(0,rect.getMaxY()));
    
    area2 = LayerColor::create(
                               defaultColor,
                               visibleSize.width,
                               rect.getMinY()
                               );
    area2->setAnchorPoint(Point(0,0));
    area2->setPosition(Point(0,0));
    
    area3 = LayerColor::create(
                               defaultColor,
                               rect.getMinX(),
                               rect.getMaxY()-rect.getMinY()
                               );
    area3->setAnchorPoint(Point(0,0));
    area3->setPosition(Point(0,rect.getMinY()));
    
    area4 = LayerColor::create(
                               defaultColor,
                               visibleSize.width - rect.getMaxX(),
                               rect.getMaxY() - rect.getMinY()
                               );
    area4->setAnchorPoint(Point(0,0));
    area4->setPosition(Point(rect.getMaxX(),rect.getMinY()));
    
    this->addChild(area1);
    this->addChild(area2);
    this->addChild(area3);
    this->addChild(area4);
}

void GuideLayer::sleep()
{
    removeListener();
}

void GuideLayer::wake()
{
    addListener();
}

void GuideLayer::sentEvent(const EventInfo& info)
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(info.getEventName(),__Integer::create(info.getEventUserData()));
}

