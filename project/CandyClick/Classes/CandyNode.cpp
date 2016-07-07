//
//  CandyNode.cpp
//  CandyClick
//
//  Created by xyh on 16/1/2.
//
//

#include "CandyNode.h"
#include "EventName.h"

#define SCALE_INTERAL   0.1f
#define Fade_INTERAL    0.3f

USING_NS_CC;
using namespace cocos2d::ui;

CandyNode* CandyNode::create(const cocos2d::Color4B& color)
{
    CandyNode *pRet = new(std::nothrow) CandyNode();
    if (pRet && pRet->init(color))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool CandyNode::init(const cocos2d::Color4B& color)
{
    initCandy(color);
    
    return true;
}

void CandyNode::initCandy(const cocos2d::Color4B& color)
{
    _layerColor = LayerColor::create(color);
    addChild(_layerColor);
    
    cocos2d::ui::Button* btn = cocos2d::ui::Button::create("res/candy.jpg");
    addChild(btn);
    btn->setScale(0.5f);
    btn->addTouchEventListener(CC_CALLBACK_2(CandyNode::onBtnClick, this));
    btn->setAnchorPoint(Point());
    btn->setOpacity(0);
    
    setContentSize(btn->getContentSize()*btn->getScale());
    _layerColor->setContentSize(getContentSize());
//    _layerColor->setContentSize(Size(110,110));
//    setContentSize(_layerColor->getContentSize());
}

void CandyNode::onEnter()
{
    cocos2d::Node::onEnter();
    
//    m_listener = EventListenerTouchOneByOne::create();
//    m_listener->setSwallowTouches(false);
//    m_listener->onTouchBegan = CC_CALLBACK_2(CandyNode::onTouchBegan,this);
//    m_listener->onTouchMoved = CC_CALLBACK_2(CandyNode::onTouchEnded,this);
//    m_listener->onTouchEnded = CC_CALLBACK_2(CandyNode::onTouchMoved,this);
//
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener,_layerColor);
}

void CandyNode::onExit()
{
//    Director::getInstance()->getEventDispatcher()->removeEventListener(m_listener);

    cocos2d::Node::onExit();
}

void CandyNode::onBtnClick(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type)
{
    CCLOG(__FUNCTION__);
    
    if (type == Widget::TouchEventType::BEGAN)
    {
        _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 0.9f));
    }
    else if (type == Widget::TouchEventType::ENDED)
    {
        _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 1.0f));
        
        notice(MSG_UI_BTN_CLICK, __Integer::create(getTag()));
    }
}

void CandyNode::flipCandy(std::function<void()> callback)
{
//    _layerColor->runAction(Sequence::create(ScaleTo::create(SCALE_INTERAL, 1.5f),ScaleTo::create(SCALE_INTERAL, 1.0f), CallFunc::create(callback), NULL));
    
    _layerColor->runAction(Sequence::create(FadeOut::create(Fade_INTERAL), FadeIn::create(Fade_INTERAL), CallFunc::create(callback), NULL));
}

bool CandyNode::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event)
{
//    _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 0.9f));
//
//    if (_layerColor->getBoundingBox().containsPoint(touch->getLocation()))
//    {
//        _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 0.9f));
//    }
    
    return true;
}

void CandyNode::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event)
{
//    if (_layerColor->getBoundingBox().containsPoint(touch->getLocation()) == false)
//    {
//        _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 1.0f));
//    }
}

void CandyNode::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event)
{
//    _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 1.0f));
//
//    if (_layerColor->getBoundingBox().containsPoint(touch->getLocation()))
//    {
//        _layerColor->runAction(ScaleTo::create(SCALE_INTERAL, 1.0f));
//        
//        notice(MSG_UI_BTN_CLICK, __Integer::create(getTag()));
//    }
}


