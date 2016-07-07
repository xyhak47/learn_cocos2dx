//
//  Facade.hpp
//  CandyClick
//
//  Created by xyh on 16/1/2.
//
//

#ifndef Facade_h
#define Facade_h

#include "cocos2d.h"

class Facade
{
public:
    static void autoSetCandyPostion(const cocos2d::Point& origin, cocos2d::Node* item, int itemNum, int rowNum, float spacing, int index);
    static cocos2d::Color4B createCandyColor(int itemNum, int index);

};


#endif /* Facade_h */
