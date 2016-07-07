//
//  Facade.cpp
//  CandyClick
//
//  Created by xyh on 16/1/2.
//
//

#include "Facade.h"


void Facade::autoSetCandyPostion(const cocos2d::Point& origin, cocos2d::Node* item, int itemNum, int rowNum, float spacing, int index)
{
    int columnNum = itemNum%rowNum == 0 ? itemNum/rowNum : itemNum/rowNum+1;
    int target_row = index/columnNum;
    int target_column = index%columnNum;
    
    const cocos2d::Size& itemSize = item->getContentSize();
    
    float delta_width = target_column*(itemSize.width+spacing);
    float delta_height = target_row*(itemSize.height+spacing);
    cocos2d::Point target_pos = origin + cocos2d::Point(delta_width, delta_height);
    item->setPosition(target_pos);
}

cocos2d::Color4B Facade::createCandyColor(int itemNum, int index)
{
    cocos2d::Color4B color = cocos2d::Color4B::BLACK;
    
    float spacing = 255/itemNum;
    
    color.r += (index * spacing);
    color.g += (index * spacing);
    color.b += (index * spacing);

    return color;
}
