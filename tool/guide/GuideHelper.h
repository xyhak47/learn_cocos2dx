//
//  GuideHelper.h
//  BotDota
//
//  Created by xyh on 15/8/24.
//
//

#ifndef __BotDota__GuideHelper__
#define __BotDota__GuideHelper__

#include "base/gcMacros.h"
#include "cocos2d.h"
#include <iostream>

class GuideHelper
{
public:
    GC_SINGLETON(GuideHelper);
    
    void saveToMapCliping(std::string name, cocos2d::Node* node);
    cocos2d::Node* getNodeFromMap(std::string name);
    
    GC_SYNTHESIZE(bool, m_inGuide, InGuide);

private:
    std::map<std::string,cocos2d::Node*>  m_mapCliping;
};

#endif /* defined(__BotDota__GuideHelper__) */
