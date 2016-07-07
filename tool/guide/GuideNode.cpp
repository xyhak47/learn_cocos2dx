//
//  GuideNode.cpp
//  BotDota
//
//  Created by xyh on 15/8/10.
//
//

#include "GuideNode.h"
#include "UI/UIResourceFile.h"
#include "components/guide/GuideEvent.h"
#include "components/guide/GuideInfo.h"
#include "Common/CocoUIUtils.h"
#include "Lua/gclua.h"
#include "Common/Facade.h"
#include "Common/Factory.h"

USING_NS_CC;

#define TALK_NODE_ZORDER  10
#define HAND_NODE_ZORDER  20
#define DIALOG_NODE_ZORDER  999

#define HAND_MOVE_SEC     2.0f

#define WORD_SEC 0.05F
#define TALK_NODE_TIMER_NAME "TalkNode"

#define WIN_SIZE                Director::getInstance()->getWinSize()
#define EGG_FRAME_POS_LEFT            Point(323,390)
#define EGG_FRAME_POS_RIGHT           Point(WIN_SIZE.width - 323,390)
#define HERO_FRAME_POS_LEFT           Point(325,456)
#define HERO_FRAME_POS_RIGHT          Point(WIN_SIZE.width - 325,456)

#define HAND_CLICK_ACTION_MOVE_DELTA   10

void GuideNode::showHand(cocos2d::Node* parent, const HandInfo& info)
{
    Sprite* hand = Sprite::createWithSpriteFrameName(I_Guide_hand);
    parent->addChild(hand, HAND_NODE_ZORDER);
    
    int offset_x, offset_y;
    if (info.getHandFlipType() == LEFT)
    {
        hand->setFlippedX(false);
        hand->setAnchorPoint(Point(0,1));
        offset_x = HAND_CLICK_ACTION_MOVE_DELTA;
        offset_y = HAND_CLICK_ACTION_MOVE_DELTA;
    }
    else
    {
        hand->setFlippedX(true);
        hand->setAnchorPoint(Point(1,1));
        offset_x = -HAND_CLICK_ACTION_MOVE_DELTA;
        offset_y = HAND_CLICK_ACTION_MOVE_DELTA;
    }
    hand->setPosition(info.getHandPos() + Point(info.getVisiableRectSize().width/2,0));
    
    auto act_1 = MoveBy::create(0.5, Vec2(-offset_x, offset_y));
    auto act_2 = MoveBy::create(0.5, Vec2(offset_x * 2, -offset_y * 2));
    auto act_3 = MoveBy::create(0.5, Vec2(-offset_x, offset_y));
    auto acts = Sequence::create(act_1, act_2, act_3, NULL);
    ActionInterval* act_repeat = RepeatForever::create(acts);
    hand->runAction(act_repeat);
}

void GuideNode::showHandMove(Node* parent, const HandInfo& info)
{
    // hand
    Sprite* hand = Sprite::createWithSpriteFrameName(I_Guide_hand);
    parent->addChild(hand, HAND_NODE_ZORDER);
    hand->setPosition(info.getHandFrom());
    Factory::createMoveByForeverTA(hand, info.getHandFrom(), info.getHandTo(), HAND_MOVE_SEC);
    
    
    // arrow
//    float distance = info.getHandTo().getDistance(info.getHandFrom());
//    int arrowNum = (int)distance/70;
//    for (int i = 0; i < arrowNum; i++)
//    {
//        Sprite* arrow = Sprite::createWithSpriteFrameName(I_Guide_arrow);
//        parent->addChild(arrow);
//        arrow->setAnchorPoint(Point(0.5, 0));
//        arrow->setPosition(info.getHandFrom());
//        float angle = info.getHandFrom().getAngle(info.getHandTo());
//        arrow->setRotation(-90 - angle*360);
//
//        CCLOG("%f",angle);
//        
//        auto func = [=]
//        {
//            Factory::createMoveByForeverTA(arrow, info.getHandFrom(), info.getHandTo(), HAND_MOVE_SEC);
//        };
//        auto call = CallFunc::create(func);
//        auto delay = DelayTime::create(0.5f * i);
//        auto acts = Sequence::create(delay,call, NULL);
//        parent->runAction(acts);
//    }
}


void GuideNode::showTalk(cocos2d::Node* parent, const TalkInfo& info)
{
    TalkNode* talk = TalkNode::create();
    talk->initData(info);
    parent->addChild(talk, TALK_NODE_ZORDER);
}

void GuideNode::showDialog(cocos2d::Node* parent, const DialogInfo& info)
{
    DialogNode* dialog = DialogNode::create();
    dialog->initData(info);
    Director::getInstance()->getRunningScene()->addChild(dialog, DIALOG_NODE_ZORDER);
}








TalkNode::TalkNode()
:m_wordCount(0)
,m_contentLength(0)
,m_type(0)
{
    addListener();
}

TalkNode::~TalkNode()
{
    removeListener();
}

void TalkNode::initData(const TalkInfo& info)
{
    setContent(info.getTalkContent());
    setType(info.getTalkType());
    setShowType(info.getTalkShowType());
    
    initContentLength();
    initUI();
    initTimer();
}

std::string TalkNode::getContentByLength(int length)
{
    if (length >= m_contentLength)
    {
        return m_content;
    }
    int i = 0;
    int index = 0;
    while(index < length)
    {
        char ch = m_content.c_str()[i];
        if (ch > -127 && ch < 0)
        {
            i+=3;
        }
        else
        {
            i++;
        }
        index++;
    }
    
    std::string str = m_content.substr(0, i);
    return str;
}

void TalkNode::updateContent(float dt)
{
    m_wordCount++;
    std::string str = getContentByLength(m_wordCount);
    m_label->setString(str);
    
    if (m_wordCount == getContentLength())
    {
        unschedule(TALK_NODE_TIMER_NAME);
        m_arrow->setVisible(false);
    }
}

void TalkNode::initContentLength()
{
    int length = (int)getContent().length();
    int i = 0;
    while(i < length)
    {
        char ch = getContent().c_str()[i];
        if (ch > -127 && ch< 0)
        {
            i+=3;
        }
        else
        {
            i++;
        }
        m_contentLength++;
    }
}

void TalkNode::initTimer()
{
    auto func = [this](float dt)
    {
        this->updateContent(dt);
    };
    
    schedule(func, WORD_SEC, TALK_NODE_TIMER_NAME);
}

void TalkNode::initUI()
{
    Size winsize = Director::getInstance()->getWinSize();
    setContentSize(winsize);
    
    Sprite* egg = Sprite::createWithSpriteFrameName(I_Guide_egg);
    Sprite* hero = Sprite::createWithSpriteFrameName(I_Guide_hero);
    Sprite* boss = Sprite::createWithSpriteFrameName(I_Guide_boos);

    
    Sprite* frame = Sprite::createWithSpriteFrameName(I_Guide_talkframe);
    
    m_label = Label::create();
    m_label->setSystemFontSize(30);
    m_label->setString("");
    m_label->setColor(Color3B(99,85,66));
    m_label->setDimensions(380, 200);
    m_label->setAlignment(TextHAlignment::LEFT);
    m_label->setPosition(Vec2(frame->getContentSize().width/2 + 10 ,frame->getContentSize().height/2 - 60));
    
    m_arrow = Sprite::createWithSpriteFrameName(I_Guide_talkarrow);
    auto act_1 = MoveBy::create(0.1, Vec2(0,2));
    auto act_2 = MoveBy::create(0.1, Vec2(0,-4));
    auto act_3 = MoveBy::create(0.1, Vec2(0,2));
    auto acts = Sequence::create(act_1, act_2, act_3, NULL);
    auto act_repeat = RepeatForever::create(acts);
    m_arrow->runAction(act_repeat);
    m_arrow->setAnchorPoint(Point(0,1));
    m_arrow->setPosition(Vec2(frame->getContentSize().width, 0) + Vec2(-117,95));
    
    frame->addChild(m_label);
    frame->addChild(m_arrow);

    // addChild by order
    addChild(egg);
    addChild(hero);
    addChild(boss);
    addChild(frame);
    
    if (getType() == EGG)
    {
        egg->setVisible(true);
        hero->setVisible(false);
        boss->setVisible(false);
        
        if (getShowType() == LEFT)
        {
            frame->setPosition(EGG_FRAME_POS_LEFT);
            frame->setFlippedX(true);
            egg->setAnchorPoint(Point(0,0));
            egg->setPosition(Point(0,0));
        }
        else
        {
            frame->setPosition(EGG_FRAME_POS_RIGHT);
            frame->setFlippedX(false);
            egg->setAnchorPoint(Point(1,0));
            egg->setPosition(Point(winsize.width,0));
            egg->setFlippedX(true);
        }
    }
    else if (getType() == HERO)
    {
        egg->setVisible(false);
        hero->setVisible(true);
        boss->setVisible(false);
        
        if (getShowType() == LEFT)
        {
            frame->setPosition(HERO_FRAME_POS_LEFT);
            frame->setFlippedX(true);
            hero->setAnchorPoint(Point(0,0));
            hero->setPosition(Point(0,0));
        }
        else
        {
            frame->setPosition(HERO_FRAME_POS_RIGHT);
            frame->setFlippedX(false);
            hero->setAnchorPoint(Point(1,0));
            hero->setPosition(Point(winsize.width,0));
            hero->setFlippedX(true);
        }
    }
    else // boss
    {
        egg->setVisible(false);
        hero->setVisible(false);
        boss->setVisible(true);
        frame->setPosition(Point(957,532));
        frame->setFlippedX(false);
        boss->setAnchorPoint(Point(1,0));
        boss->setPosition(Point(winsize.width,0));
        boss->setFlippedX(false);
    }
}

void TalkNode::addListener()
{
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->onTouchBegan = CC_CALLBACK_2(TalkNode::onTouchBegan,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener,this);
}

void TalkNode::removeListener()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_listener);
}

bool TalkNode::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event)
{
    if (m_wordCount < getContentLength())
    {
        unschedule(TALK_NODE_TIMER_NAME);
        m_wordCount = getContentLength();
        std::string str = getContentByLength(m_wordCount);
        m_label->setString(str);
        m_arrow->setVisible(false);
    }
    else if (m_wordCount == getContentLength())
    {
        DISPATCH_EVENT(GUIDE_EVENT_TALK_TOUCH);
    }
    
    return false;
}





DialogNode::DialogNode()
{
}

DialogNode::~DialogNode()
{
    
}

void DialogNode::initUI()
{
    setContentSize(WIN_SIZE);
    
    Sprite* bg = Sprite::createWithSpriteFrameName(I_Common_bg_1116);
    addChild(bg);
    bg->setPosition(WIN_SIZE/2);
    bg->setScale(0.5);
    

    Button* btnKnow = Button::create(I_Common_btnKnow, I_Common_btnKnow_On, "", Button::TextureResType::PLIST);
    Point pos_of_btn = bg->getPosition() + Point(0, -bg->getContentSize().height/4 + btnKnow->getContentSize().height/2);

    btnKnow->setPosition(pos_of_btn);
    addChild(btnKnow);
    auto func_close = [this](Ref* target)
    {
        this->btnCloseClick(target);
    };
    btnKnow->addClickEventListener(func_close);
    
    
    Button* btnSee = Button::create(I_Common_btnGoto, I_Common_btnGoto_On, "", Button::TextureResType::PLIST);
    btnSee->setPosition(pos_of_btn);
    addChild(btnSee);
    auto func_see = [this](Ref* target)
    {
        this->btnSeeClick(target);
    };
    btnSee->addClickEventListener(func_see);
    
    if (Facade::isInGuide())
    {
        Facade::saveCliping(btnSee, GUIDE_CLIPING_DIALOG_SEE);
    }
    
    if (m_type == SEE)
    {
        btnSee->setVisible(true);
        btnKnow->setVisible(false);
    }
    else
    {
        btnSee->setVisible(false);
        btnKnow->setVisible(true);
    }
    
    m_label = Label::create();
    m_label->setPosition(bg->getPosition());
    m_label->setSystemFontSize(30);
    m_label->setString(m_content);
    m_label->setColor(Color3B(99,85,66));
    m_label->setDimensions(380, 200);
    m_label->setAlignment(TextHAlignment::CENTER);
    addChild(m_label);
}

void DialogNode::btnCloseClick(cocos2d::Ref* target)
{
    removeFromParent();
}

void DialogNode::btnSeeClick(cocos2d::Ref* target)
{
    if (m_firstTime == true)
    {
        if (Facade::isInGuide())
        {
            Facade::dispatchGuideContinue();
            removeFromParent(); // parent is running scene
            return;
        }
    }
    
    if (GUIDE_THEME_SUMMON == m_theme)
    {
        gclua::goLua(LUA_GO_SUMMON);
    }
    
    if (GUIDE_THEME_HEROLIST == m_theme
        || GUIDE_THEME_FRIEND == m_theme
        )
    {
        Facade::fadeInHall();
    }
}

void DialogNode::initData(const DialogInfo& info)
{
    m_content = info.getContent();
    m_theme = info.getName();
    m_type = info.getType();
    m_firstTime = info.getFirstTime();
    
    initUI();
}



