//
//  CandyClickScene.hpp
//  CandyClick
//
//  Created by xyh on 15/12/31.
//
//

#ifndef CandyClickScene_h
#define CandyClickScene_h

#include "cocos2d.h"

class CandyClickScene : public cocos2d::Scene
{
public:
    CREATE_FUNC(CandyClickScene);
    
protected:
    CandyClickScene();
    ~CandyClickScene();
    virtual bool init() override;
};

#endif /* CandyClickScene_h */
