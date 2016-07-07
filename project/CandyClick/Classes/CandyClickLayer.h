//
//  CandyClickLayer.hpp
//  CandyClick
//
//  Created by xyh on 15/12/31.
//
//

#ifndef CandyClickLayer_hpp
#define CandyClickLayer_hpp

#include "cocos2d.h"
#include "CandyNode.h"

class CandyClickLayer : public cocos2d::Layer, public EventHandler
{
private:
    enum
    {
        CANDY_NUM = 25,
        CANDY_ROW_NUM = 5,
        CANDY_ROW_SPACING = 5,
        
        WIN_STEP_NUM = 100,
        
        ZORDER_BG = 0,
        ZORDER_UI = 1,
        ZORDER_GAME = 2,
    };
    
    enum TURN
    {
        PLAYER = 0,
        COMPUTER,
    };
    
public:
    CREATE_FUNC(CandyClickLayer);
    
protected:
    CandyClickLayer();
    ~CandyClickLayer();
    virtual bool init() override;
    virtual void handleEvent(cocos2d::EventCustom* event) override;
    virtual void onEnter() override;

private:
    void initBg();
    void initCandy();
    
    void onCandyClick(cocos2d::EventCustom* event);
    
    void resetGame();
    int randomCandy();
    bool advanceGame();
    
    void win();
    void lose();
    
    void check(int index);

    void playSequece(int index);
    
    void resetRecord();
    void resetScore();
    void clearScore();

private:
    std::map<int, CandyNode*> mapCandy;
    std::vector<int> vecStep;
    TURN _currentTurn;
    int indexOfNextCandyToTouch;
    bool isInGame;
    cocos2d::Label* _scorePlayer;
    cocos2d::Label* _scoreBest;

};
#endif /* CandyClickLayer_hpp */
