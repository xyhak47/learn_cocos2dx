//
//  StartScene.hpp
//  CandyClick
//
//  Created by xyh on 16/1/4.
//
//

#ifndef StartScene_h
#define StartScene_h

#include "cocos2d.h"

class StartScene : public cocos2d::Scene
{
public:
    CREATE_FUNC(StartScene);
    
protected:
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
private:
    void enterGame();
    
private:
    cocos2d::Label* _label;
};

#endif /* StartScene_h */
