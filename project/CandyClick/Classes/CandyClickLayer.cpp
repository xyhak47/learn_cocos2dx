//
//  CandyClickLayer.cpp
//  CandyClick
//
//  Created by xyh on 15/12/31.
//
//

#include "CandyClickLayer.h"
#include "Facade.h"
#include "EventName.h"
#include "ui/UIScale9Sprite.h"

#define CANDY_ORGIN  cocos2d::Point(200,50)
#define RECORE  "RECORE"

USING_NS_CC;
using namespace cocos2d::ui;

CandyClickLayer::CandyClickLayer()
:indexOfNextCandyToTouch(0)
,mapCandy()
,vecStep()
,_currentTurn(COMPUTER)
,isInGame(false)
,_scoreBest(nullptr)
,_scorePlayer(nullptr)
{

}

CandyClickLayer::~CandyClickLayer()
{
    
}

bool CandyClickLayer::init()
{
    initCandy();
    
    return true;
}

void CandyClickLayer::onEnter()
{
    cocos2d::Layer::onEnter();
    
    //add begin game btn
    Button* btnBegin = Button::create();
    addChild(btnBegin, ZORDER_UI);
    btnBegin->addClickEventListener([this](Ref*){ resetGame(); });
    btnBegin->setPosition(Point(950,100));
    btnBegin->setTitleColor(Color3B::BLACK);
    btnBegin->setTitleText("Let's Begin");
    btnBegin->setTitleFontName("Zapfino");
    btnBegin->setTitleFontSize(30);

    // add describtion label
    Label* desc_label = Label::createWithSystemFont("How to Play:\n Click in Order\n Every Time", "Zapfino", 30);
    addChild(desc_label, ZORDER_UI);
    desc_label->setPosition(950,400);
    desc_label->setTextColor(Color4B::BLACK);
    
    // add bg
    Scale9Sprite* bg = Scale9Sprite::create("res/bg.jpg");
    addChild(bg, ZORDER_BG);
    bg->setAnchorPoint(Point());
    bg->setPreferredSize(Director::getInstance()->getWinSize());
    
    
    // add score area
    _scorePlayer = Label::createWithSystemFont("Score:0", "Zapfino", 20);
    addChild(_scorePlayer, ZORDER_UI);
    _scorePlayer->setPosition(10, 400);
    _scorePlayer->setTextColor(Color4B::BLACK);
    _scorePlayer->setAnchorPoint(Point());

    _scoreBest = Label::createWithSystemFont("Best:0", "Zapfino", 20);
    addChild(_scoreBest, ZORDER_UI);
    _scoreBest->setPosition(10, 350);
    _scoreBest->setTextColor(Color4B::BLACK);
    _scoreBest->setAnchorPoint(Point());
    
//    resetRecord();
}

void CandyClickLayer::initBg()
{
    LayerColor* color = LayerColor::create(Color4B::WHITE);
    color->setContentSize(Director::getInstance()->getWinSize());
    addChild(color);
}

void CandyClickLayer::initCandy()
{
    for (int i = 0; i < CANDY_NUM; i++)
    {
        CandyNode* candy = CandyNode::create(Facade::createCandyColor(CANDY_NUM, i));
        addChild(candy, ZORDER_GAME);
        candy->setTag(i);
        mapCandy[i] = candy;
        candy->setEventHandler(this);
        
        Facade::autoSetCandyPostion(CANDY_ORGIN, candy, CANDY_NUM, CANDY_ROW_NUM, CANDY_ROW_SPACING, i);
    }
}

void CandyClickLayer::handleEvent(cocos2d::EventCustom* event)
{
    if (event->getEventName() == MSG_UI_BTN_CLICK)
    {
        onCandyClick(event);
    }
}

void CandyClickLayer::onCandyClick(cocos2d::EventCustom* event)
{
    CCLOG(__FUNCTION__);
    
    if (isInGame == false)
    {
        return;
    }
    
    // 电脑时，屏蔽点击/
    if (_currentTurn == COMPUTER)
    {
        return;
    }
    
    __Integer* tag = (__Integer*)event->getUserData();
    check(tag->getValue());
}

void CandyClickLayer::resetGame()
{
    vecStep.clear();
    advanceGame();
    isInGame = true;
}

int CandyClickLayer::randomCandy()
{
    return random<int>(0, (int)CANDY_NUM-1);
}

bool CandyClickLayer::advanceGame()
{
    bool result = true;
    
    if (vecStep.size() == WIN_STEP_NUM)
    {
        result = false;
    }
    else
    {
        vecStep.push_back(randomCandy());
        
        playSequece(0);
    }
    
    return result;
}

void CandyClickLayer::win()
{
    MessageBox("once more?", "you win!");
    isInGame = false;
//    resetRecord();
}

void CandyClickLayer::lose()
{
    MessageBox("try again?", "you lose!");
    isInGame = false;
//    clearScore();
}

void CandyClickLayer::check(int index)
{
    CandyNode* candy = mapCandy[index];
    if (candy)
    {
        if (index == vecStep[indexOfNextCandyToTouch])
        {
            indexOfNextCandyToTouch++;
            
            if (indexOfNextCandyToTouch == vecStep.size())
            {
                if (advanceGame() == false)
                {
                    win();
                }
                
                indexOfNextCandyToTouch = 0;
            }
        }
        else
        {
            lose();
            indexOfNextCandyToTouch = 0;
        }
    }
}

void CandyClickLayer::playSequece(int index)
{
    _currentTurn = COMPUTER;
    
    if (index == vecStep.size())
    {
        _currentTurn = PLAYER;
        return;
    }
    
    CandyNode* candy = mapCandy[vecStep.at(index)];
    candy->flipCandy( [=]{playSequece(index+1);} );
}

void CandyClickLayer::resetRecord()
{
    int record = UserDefault::getInstance()->getIntegerForKey(RECORE);
    
    if (indexOfNextCandyToTouch > record)
    {
        record = indexOfNextCandyToTouch;
        UserDefault::getInstance()->setIntegerForKey(RECORE, record);
        UserDefault::getInstance()->flush();
    }
    
    _scoreBest->setString(StringUtils::format("Best:%d",record));
}

void CandyClickLayer::resetScore()
{
    _scorePlayer->setString(StringUtils::format("Score:%d",indexOfNextCandyToTouch));
}

void CandyClickLayer::clearScore()
{
    _scorePlayer->setString("Score:0");
}




