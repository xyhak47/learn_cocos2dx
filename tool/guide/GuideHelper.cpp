//
//  GuideHelper.cpp
//  BotDota
//
//  Created by xyh on 15/8/24.
//
//

#include "GuideHelper.h"

USING_NS_CC;

GuideHelper::GuideHelper()
:m_inGuide(false)
{
}


void GuideHelper::saveToMapCliping(std::string name, cocos2d::Node* node)
{
    m_mapCliping[name] = node;
}

Node* GuideHelper::getNodeFromMap(std::string name)
{
    return m_mapCliping[name];
}


