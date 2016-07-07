//
//  CandyNode.hpp
//  CandyClick
//
//  Created by xyh on 16/1/2.
//
//

#ifndef CandyNode_h
#define CandyNode_h

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "EventHandler.h"

class CandyNode : public cocos2d::Node, public EventHandler
{
private:
    enum
    {
        CANDY_WIDTH = 100,
    };
    
    
public:
    static CandyNode* create(const cocos2d::Color4B& color);
    virtual bool init(const cocos2d::Color4B& color);
    void flipCandy(std::function<void()> callback);
    
protected:
    virtual void onEnter();
    virtual void onExit();

private:
    void initCandy(const cocos2d::Color4B& color);
    void onBtnClick(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);
    bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);

private:
    cocos2d::EventListenerTouchOneByOne* m_listener;
    cocos2d::LayerColor* _layerColor;
};

#endif /* CandyNode_h */
