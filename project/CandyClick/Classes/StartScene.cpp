//
//  StartScene.cpp
//  CandyClick
//
//  Created by xyh on 16/1/4.
//
//

#include "StartScene.h"
#include "ui/UIScale9Sprite.h"
#include "CandyClickScene.h"

#define FADE_INTERAL    4.0f
#define LOGO_FONT       60

using namespace cocos2d::ui;

USING_NS_CC;

void StartScene::onEnter()
{
    cocos2d::Scene::onEnter();
    
    Size winsize = Director::getInstance()->getWinSize();
    
    // add bg
    Scale9Sprite* bg = Scale9Sprite::create("res/beginbg.jpg");
    addChild(bg);
    bg->setAnchorPoint(Point());
    bg->setPreferredSize(winsize);
    
    // add describtion label
    _label = Label::createWithSystemFont("Ying · Mo", "Zapfino", LOGO_FONT);
    addChild(_label);
    _label->setTextColor(Color4B::BLACK);
    _label->setPosition(winsize/2);
    _label->setOpacity(0);
    
    // add ParticleSystem
    ParticleSystem* particleSystem = ParticleRain::create();
    particleSystem->setTexture(Director::getInstance()->getTextureCache()->addImage("res/flower.jpg"));
    addChild(particleSystem);
}

void StartScene::onEnterTransitionDidFinish()
{
    cocos2d::Scene::onEnterTransitionDidFinish();
    
    CallFunc* call = CallFunc::create( [=]{ enterGame();} );
    _label->runAction(Sequence::create(FadeIn::create(FADE_INTERAL),FadeOut::create(FADE_INTERAL), call, NULL));
}

void StartScene::enterGame()
{
    Scene* pScene = TransitionFade::create(0.8f, CandyClickScene::create(), Color3B::WHITE);
    Director::getInstance()->replaceScene(pScene);
}

