#ifndef _GUIDELAYER_H_
#define _GUIDELAYER_H_

#include "cocos2d.h"
#include "components/guide/GuideInfo.h"

class  GuideLayer : public cocos2d::Node
{
private:
	GuideLayer();
    ~GuideLayer();
    
public:
	static GuideLayer* create(const cocos2d::Rect& rect);
	static GuideLayer* create(const cocos2d::Rect& rect,const cocos2d::Color4B& color);

    void wait();
    void reset();
    void resetCliping(const RectInfo& rectInfo);
    void showHandClick(const HandInfo& info);
    void showHandMove(const HandInfo& info);
    void showTalk(const TalkInfo& info);
    void showDialog(const DialogInfo& info);
    void sleep();
    void wake();
    void sentEvent(const EventInfo& info);
    
private:
	bool init(const cocos2d::Rect& rect);
	bool init(const cocos2d::Rect& rect,const cocos2d::Color4B& color);
	bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    void addMask(bool show = true);
    void addCliping(const RectInfo& rectInfo);
    void addListener();
    void removeListener();
    void addMaskAround();

private:
	cocos2d::LayerColor* area1;
	cocos2d::LayerColor* area2;
	cocos2d::LayerColor* area3;
	cocos2d::LayerColor* area4;

	cocos2d::Color4B defaultColor;

	cocos2d::Rect m_rect;
	cocos2d::EventListenerTouchOneByOne* m_listener;
    
    bool m_exitHand;
};

#endif