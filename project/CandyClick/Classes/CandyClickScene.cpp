//
//  CandyClickScene.cpp
//  CandyClick
//
//  Created by xyh on 15/12/31.
//
//

#include "CandyClickScene.h"
#include "CandyClickLayer.h"

CandyClickScene::CandyClickScene()
{
    
}

CandyClickScene::~CandyClickScene()
{
    
}

bool CandyClickScene::init()
{
    addChild(CandyClickLayer::create());
    
    return true;
}


